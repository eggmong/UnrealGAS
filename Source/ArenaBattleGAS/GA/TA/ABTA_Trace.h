// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ABTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AABTA_Trace();

	// 즉시 물리 판정을 수행하기 때문에 cancel 은 구현 X

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	// 타겟팅 완료 되면 물리 판정하기 위한 함수
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
