// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"

UABAT_Trace::UABAT_Trace()
{
}

UABAT_Trace* UABAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AABTA_Trace> TargetActorClass)
{
	// 타겟 액터 클래스를 인자로 받았을 때, 이 클래스 정보로 하여금 spawn 타겟 액터를 시켜주도록 명령내림

	UABAT_Trace* NewTask = NewAbilityTask<UABAT_Trace>(OwningAbility);

	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UABAT_Trace::Activate()
{
	Super::Activate();

	// 일정 시간이 지난 후 AT를 종료하려고
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
		// true 라면, FTraceResultDelegate OnComplete 를 구독한 GA에게 알려준다

		// 타겟 액터로부터 확인이 끝났을 때, 브로드캐스트를 받았을 때 AbilityTask는 종료 되어야 함
		// ( 받는 브로드캐스트 : TargetDataReadyDelegate.Broadcast(DataHandle); )
		// 종료되며 브로드캐스트 하여 GA에게 타겟 정보를 넘겨준다.
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
