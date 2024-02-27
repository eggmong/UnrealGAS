// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Rotate.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/ABGameplayTag.h"

UABGA_Rotate::UABGA_Rotate()
{
	AbilityTags.AddTag(ABTAG_ACTOR_ROTATE);		// 이 어빌리티를 대표하는 태그 추가
	ActivationOwnedTags.AddTag(ABTAG_ACTOR_ISROTATING);				// ActivationOwnedTags : 활성화될 때 발동되는 태그.
																	// 상태 태그 추가
}

/// <param name="Handle">현재 발동한 어빌리티를 우리가 직접 처리할 수 있는 핸들 정보가 들어옴</param>
/// <param name="ActorInfo">AABGASFountain 에서 InitAbilityActorInfo 해줄 때 넣었던 오너 인포, 아바타 인포를 담은 구조체</param>
/// <param name="ActivationInfo">어떻게 발동했는지에 대한 정보를 담은 구조체</param>
/// <param name="TriggerEventData">외부에서 발동했다면 어떻게 발동시켰는지에 대한 정보를 담은 구조체</param>
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
