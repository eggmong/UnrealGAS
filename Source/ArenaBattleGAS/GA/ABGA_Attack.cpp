// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Attack.h"
#include "Tag/ABGameplayTag.h"
#include "Character/ABCharacterBase.h"		// npc 캐릭터에도 어빌리티를 추가할 가능성이 있기 때문에
											// 부모 클래스로 상속 받음
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ABComboActionData.h"

UABGA_Attack::UABGA_Attack()
{
	// 인스턴스 폴리싱 지정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;


}

void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 어빌리티가 실행될 때 몽타주 재생하도록,
	// 먼저 몽타주 실행에 필요한 아바타를 가져오기 위해 가져온 후 캐릭터로 형 변환
	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());

	// 콤보 데이터 가져오기
	CurrentComboData = ABCharacter->GetComboActionData();

	// Attack 어빌리티 발동될 때 Move 멈추고, Attack 끝나면 다시 움직이도록 수정
	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


	// 모션을 재생하도록 명령을 내리는 'Ability Task'
	// 어빌리티 태스크가 생성되지만, 바로 실행 되는 것은 아니다.
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage(), 1.0f, GetNextSection());
	// OwningAbility : 이 태스크를 소유하고 있는 어빌리티
	// TaskInstanceName : 이 태스크를 가리킬 수 있는 고유 이름
	// MontageToPlay : 캐릭터에서 제공 받을 몽타주 에셋

	// 태스크가 종료될 때 실행될 함수를 델리게이트(OnCompleted)에 바인딩해준다
	PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompleteCallback);

	// Interrupt 발생했을 때의 함수도 바인딩해줌
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UABGA_Attack::OnInterruptedCallback);

	// 이걸 호출해줘야 태스크가 실행된다.
	PlayAttackTask->ReadyForActivation();

	StartComboTimer();
}

void UABGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UABGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());

	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;
}

void UABGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	if (!ComboTimerHandle.IsValid())
	{
		// 콤보 타이머 핸들이 유효하지 않으면 = 핸들 없는데 입력이 들어왔다는 것 -> 마지막 콤보
		
		HasNextComboInput = false;
		// 마지막 콤보일 땐 타이머가 발동되지 않도록
	}
	else
	{
		// 하지만 타이머 핸들이 유효하다면 = 현재 돌아가고 있다면

		HasNextComboInput = true;
	}
}

void UABGA_Attack::OnCompleteCallback()
{
	// 리플리케이션 옵션. 사용 안할거지만...
	bool bReplicatedEndAbility = true;

	// 취소된건지? -> 잘 완료 된거니까 아니라서 false
	bool bWasCancelled = false;

	// 어빌리티가 다 끝났다고 상태 지정
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UABGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);

	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);

	return NextSection;
}

void UABGA_Attack::StartComboTimer()
{
	// 콤보 타이머 발동 함수

	// 콤보가 1일 때의 콤보 데이터 값을 가져와야 하는데 데이터는 0부터 시작하므로 -1 해줌
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));		// 데이터 유무 체크

	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// 입력이 들어왔을 때 
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UABGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UABGA_Attack::CheckComboInput()
{
	// 제한시간 안에 콤보 입력이 들어왔는지 체크

	// 타이머가 발동되면 일단 핸들 무력화
	ComboTimerHandle.Invalidate();

	if (HasNextComboInput)
	{
		// 콤보 인풋이 있으면 다음 섹션으로 이동 (다음 섹션의 애니 재생)
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
