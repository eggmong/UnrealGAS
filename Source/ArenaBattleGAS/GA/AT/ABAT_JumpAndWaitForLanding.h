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
	/// <param name="OwningAbility">�ڱ⸦ �����ϰ� �ִ� GameplayAbility</param>
	/// <returns>�ڱ� �ڽ��� ��ȯ</returns>
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	// �ߵ� �� �� ȣ��
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	FJumpAndWaitForLandingDelegate OnComplete;

};
