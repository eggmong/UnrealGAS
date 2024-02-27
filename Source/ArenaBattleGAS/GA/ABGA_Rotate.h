// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ABGA_Rotate.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGA_Rotate : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	// Gameplay Tag 등록하기 위해 생성자 생성
	UABGA_Rotate();
	
public:
	// UGameplayAbility 들어가보면 선언되어 있는 함수 중 ActivateAbility, CancelAbility 재정의 할거임.

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
};
