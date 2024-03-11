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

	/// <param name="OwningAbility">�ڱ⸦ �����ϰ� �ִ� GameplayAbility</param>
	/// <returns>�ڱ� �ڽ��� ��ȯ</returns>
	// UFUNCTION(BlueprintCallable) �� �����ϸ� �������Ʈ���� �ν��ؼ� ��� ����
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	// �ߵ� �� �� ȣ��
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	// BlueprintAssignable : �������Ʈ���� ����� �� �ֵ��� 
	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;

protected:

	// ACharacter ������, FLandedSignature LandedDelegate; ��� ��������Ʈ�� ����.
	// FLandedSignature�� DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLandedSignature, const FHitResult&, Hit); ��� ����Ǿ� ����.
	// ���⿡�� �Ȱ��� �����ؼ� ������ ������ ��Ȳ�� AT�� �ľ��� �� �ֵ��� ��
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);

};
