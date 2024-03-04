// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Attack.h"
#include "Tag/ABGameplayTag.h"
#include "Character/ABCharacterBase.h"		// npc ĳ���Ϳ��� �����Ƽ�� �߰��� ���ɼ��� �ֱ� ������
											// �θ� Ŭ������ ��� ����
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ABComboActionData.h"

UABGA_Attack::UABGA_Attack()
{
	// �ν��Ͻ� ������ ����
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;


}

void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// �����Ƽ�� ����� �� ��Ÿ�� ����ϵ���,
	// ���� ��Ÿ�� ���࿡ �ʿ��� �ƹ�Ÿ�� �������� ���� ������ �� ĳ���ͷ� �� ��ȯ
	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());

	// �޺� ������ ��������
	CurrentComboData = ABCharacter->GetComboActionData();

	// Attack �����Ƽ �ߵ��� �� Move ���߰�, Attack ������ �ٽ� �����̵��� ����
	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


	// ����� ����ϵ��� ����� ������ 'Ability Task'
	// �����Ƽ �½�ũ�� ����������, �ٷ� ���� �Ǵ� ���� �ƴϴ�.
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage(), 1.0f, GetNextSection());
	// OwningAbility : �� �½�ũ�� �����ϰ� �ִ� �����Ƽ
	// TaskInstanceName : �� �½�ũ�� ����ų �� �ִ� ���� �̸�
	// MontageToPlay : ĳ���Ϳ��� ���� ���� ��Ÿ�� ����

	// �½�ũ�� ����� �� ����� �Լ��� ��������Ʈ(OnCompleted)�� ���ε����ش�
	PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompleteCallback);

	// Interrupt �߻����� ���� �Լ��� ���ε�����
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UABGA_Attack::OnInterruptedCallback);

	// �̰� ȣ������� �½�ũ�� ����ȴ�.
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
		// �޺� Ÿ�̸� �ڵ��� ��ȿ���� ������ = �ڵ� ���µ� �Է��� ���Դٴ� �� -> ������ �޺�
		
		HasNextComboInput = false;
		// ������ �޺��� �� Ÿ�̸Ӱ� �ߵ����� �ʵ���
	}
	else
	{
		// ������ Ÿ�̸� �ڵ��� ��ȿ�ϴٸ� = ���� ���ư��� �ִٸ�

		HasNextComboInput = true;
	}
}

void UABGA_Attack::OnCompleteCallback()
{
	// ���ø����̼� �ɼ�. ��� ���Ұ�����...
	bool bReplicatedEndAbility = true;

	// ��ҵȰ���? -> �� �Ϸ� �ȰŴϱ� �ƴ϶� false
	bool bWasCancelled = false;

	// �����Ƽ�� �� �����ٰ� ���� ����
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
	// �޺� Ÿ�̸� �ߵ� �Լ�

	// �޺��� 1�� ���� �޺� ������ ���� �����;� �ϴµ� �����ʹ� 0���� �����ϹǷ� -1 ����
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));		// ������ ���� üũ

	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		// �Է��� ������ �� 
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UABGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UABGA_Attack::CheckComboInput()
{
	// ���ѽð� �ȿ� �޺� �Է��� ���Դ��� üũ

	// Ÿ�̸Ӱ� �ߵ��Ǹ� �ϴ� �ڵ� ����ȭ
	ComboTimerHandle.Invalidate();

	if (HasNextComboInput)
	{
		// �޺� ��ǲ�� ������ ���� �������� �̵� (���� ������ �ִ� ���)
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
