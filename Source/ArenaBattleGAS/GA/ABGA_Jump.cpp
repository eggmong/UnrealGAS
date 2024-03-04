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

	// CastChecked �Ƚᵵ, ���� ActorInfo�� Pawn�� ����ִٸ�, Character�� null �ϰ���.
	// �� ������ return ������ �ɷ����� �� ����. �׷��� CastChecked �� �� ��.
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());

	// ĳ���Ͱ� �ְ�, ĳ���Ͱ� ������ �������� Ȯ���ؼ� ����
	return (Character && Character->CanJump());
}

void UABGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// Ű �Է� ���� ��

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

	Character->StopJumping();
}
