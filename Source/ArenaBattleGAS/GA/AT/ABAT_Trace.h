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

	// Ÿ�� ���� Ŭ����(ABTA_Trace)�� ���ڷ� �޾��� ��, �� Ŭ���� ������ �Ͽ��� ���� ���͸� �����ֵ��� ��ɳ���
	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AABTA_Trace> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	// Ÿ�� ����TA ����
	void SpawnAndInitializeTargetActor();

	// TA�� Ÿ���� ��� ��������Ʈ ���� �� �������� �Լ�
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	// CreateTask ���� ���� Ÿ�پ��� �����ϱ� ���� ������ ����

	// ������ Ŭ���� ���� ����
	UPROPERTY()
	TSubclassOf<class AABTA_Trace> TargetActorClass;

	// Ÿ�� ���� ���������� �� ������ �� �ִ� ����
	UPROPERTY()
	TObjectPtr<class AABTA_Trace> SpawnedTargetActor;
};
