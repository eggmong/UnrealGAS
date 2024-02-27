// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Rotate.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/ABGameplayTag.h"

UABGA_Rotate::UABGA_Rotate()
{
	AbilityTags.AddTag(ABTAG_ACTOR_ROTATE);		// �� �����Ƽ�� ��ǥ�ϴ� �±� �߰�
	ActivationOwnedTags.AddTag(ABTAG_ACTOR_ISROTATING);				// ActivationOwnedTags : Ȱ��ȭ�� �� �ߵ��Ǵ� �±�.
																	// ���� �±� �߰�
}

/// <param name="Handle">���� �ߵ��� �����Ƽ�� �츮�� ���� ó���� �� �ִ� �ڵ� ������ ����</param>
/// <param name="ActorInfo">AABGASFountain ���� InitAbilityActorInfo ���� �� �־��� ���� ����, �ƹ�Ÿ ������ ���� ����ü</param>
/// <param name="ActivationInfo">��� �ߵ��ߴ����� ���� ������ ���� ����ü</param>
/// <param name="TriggerEventData">�ܺο��� �ߵ��ߴٸ� ��� �ߵ����״����� ���� ������ ���� ����ü</param>
void UABGA_Rotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));

		if (RotatingMovement)
		{
			RotatingMovement->Activate(true);
		}
	}
}

void UABGA_Rotate::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));

		if (RotatingMovement)
		{
			RotatingMovement->Deactivate();
		}
	}
}
