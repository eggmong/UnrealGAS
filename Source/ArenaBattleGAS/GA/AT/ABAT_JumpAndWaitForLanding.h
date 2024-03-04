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

	/// <param name="OwningAbility">자기를 소유하고 있는 GameplayAbility</param>
	/// <returns>자기 자신을 반환</returns>
	// UFUNCTION(BlueprintCallable) 을 지정하면 블루프린트에서 인식해서 사용 가능
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	// 발동 될 때 호출
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	// BlueprintAssignable : 블루프린트에서 사용할 수 있도록 
	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;

protected:

	// ACharacter 들어가보면, FLandedSignature LandedDelegate; 라고 델리게이트가 있음.
	// FLandedSignature는 DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLandedSignature, const FHitResult&, Hit); 라고 선언되어 있음.
	// 여기에도 똑같이 선언해서 점프가 끝나는 상황을 AT가 파악할 수 있도록 함
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);

};
