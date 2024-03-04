// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UABAT_JumpAndWaitForLanding();

protected:
	/// <param name="OwningAbility">자기를 소유하고 있는 GameplayAbility</param>
	/// <returns>자기 자신을 반환</returns>
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	// 발동 될 때 호출
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	FJumpAndWaitForLandingDelegate OnComplete;

};
