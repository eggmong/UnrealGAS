// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"

UABAT_Trace::UABAT_Trace()
{
}

UABAT_Trace* UABAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AABTA_Trace> TargetActorClass)
{
	// Ÿ�� ���� Ŭ������ ���ڷ� �޾��� ��, �� Ŭ���� ������ �Ͽ��� spawn Ÿ�� ���͸� �����ֵ��� ��ɳ���

	UABAT_Trace* NewTask = NewAbilityTask<UABAT_Trace>(OwningAbility);

	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UABAT_Trace::Activate()
{
	Super::Activate();

	// ���� �ð��� ���� �� AT�� �����Ϸ���
	SetWaitingOnAvatar();
}

void UABAT_Trace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void UABAT_Trace::SpawnAndInitializeTargetActor()
{
	
}

void UABAT_Trace::FinalizeTargetActor()
{
}

void UABAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// true ���, FTraceResultDelegate OnComplete �� ������ GA���� �˷��ش�

		// Ÿ�� ���ͷκ��� Ȯ���� ������ ��, ��ε�ĳ��Ʈ�� �޾��� �� AbilityTask�� ���� �Ǿ�� ��
		// ( �޴� ��ε�ĳ��Ʈ : TargetDataReadyDelegate.Broadcast(DataHandle); )
		// ����Ǹ� ��ε�ĳ��Ʈ �Ͽ� GA���� Ÿ�� ������ �Ѱ��ش�.
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
