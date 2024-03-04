// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Jump.h"
#include "GameFramework/Character.h"

UABGA_Jump::UABGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


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
