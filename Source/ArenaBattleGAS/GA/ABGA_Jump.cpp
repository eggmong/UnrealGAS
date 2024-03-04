// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Jump.h"
#include "GameFramework/Character.h"
#include "GA/AT/ABAT_JumpAndWaitForLanding.h"

UABGA_Jump::UABGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 블루프린트로 만든 BPGA_Jump 에서 동작하도록 하려고 주석처리 함.
	/*UABAT_JumpAndWaitForLanding* JumpAndWaitingForLandingTask = UABAT_JumpAndWaitForLanding::CreateTask(this);
	JumpAndWaitingForLandingTask->OnComplete.AddDynamic(this, &UABGA_Jump::OnLandedCallback);
	JumpAndWaitingForLandingTask->ReadyForActivation();*/
}

bool UABGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (!bResult)
	{
		return false;
	}

	// CastChecked 안써도, 만약 ActorInfo에 Pawn이 들어있다면, Character는 null 일것임.
	// 즉 마지막 return 값에서 걸러지게 될 것임. 그래서 CastChecked 안 쓴 것.
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	// 캐릭터가 있고, 캐릭터가 점프가 가능한지 확인해서 리턴
	return (Character && Character->CanJump());
}

void UABGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// 키 입력 뗐을 때

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

	Character->StopJumping();
}

void UABGA_Jump::OnLandedCallback()
{
	// 델리게이트로 인해 땅에 닿았을 때 호출될 함수
	// 공식적으로 점프 상태가 종료된 것이다! -> EndAbility 발동 필요

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	// 어빌리티가 다 끝났다고 상태 지정
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
