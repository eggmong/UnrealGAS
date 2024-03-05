// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UABAT_Trace();

	// 타겟 액터 클래스(ABTA_Trace)를 인자로 받았을 때, 이 클래스 정보로 하여금 스폰 액터를 시켜주도록 명령내림
	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AABTA_Trace> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	// 타겟 액터TA 생성
	void SpawnAndInitializeTargetActor();

	// TA의 타겟팅 결과 델리게이트 구독 및 마무리할 함수
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	// CreateTask 에서 받은 타겟액터 저장하기 위한 변수들 생성

	// 생성한 클래스 정보 저장
	UPROPERTY()
	TSubclassOf<class AABTA_Trace> TargetActorClass;

	// 타겟 액터 스폰시켰을 때 저장할 수 있는 액터
	UPROPERTY()
	TObjectPtr<class AABTA_Trace> SpawnedTargetActor;
};
