// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"
#include "AbilitySystemComponent.h"

UABAT_Trace::UABAT_Trace()
{
}

UABAT_Trace* UABAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AABTA_Trace> TargetActorClass)
{
	UABAT_Trace* NewTask = NewAbilityTask<UABAT_Trace>(OwningAbility);

	// 타겟 액터 클래스를 인자로 받았을 때, 어빌리티태스크(AT)에게 타겟 액터를 스폰 하도록 함
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UABAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	// 일정 시간이 지난 후(지연시킨 다음) AT를 종료하려고
	SetWaitingOnAvatar();
}

void UABAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		// Task가 종료될 때, 현재 스폰된 TargetActor가 있다면 삭제 처리
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UABAT_Trace::SpawnAndInitializeTargetActor()
{
	// 타겟 액터 생성

	SpawnedTargetActor = Cast<AABTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UABAT_Trace::OnTargetDataReadyCallback);

		// TargetActor에서 브로드캐스트 하는 TargetDataReadyDelegate 델리게이트를 구독하여
		// OnTargetDataReadyCallback 함수를 바인딩 한다.
		// 그러면 브로드캐스트 될 때 OnTargetDataReadyCallback 함수가 호출될 것.
	}
}

void UABAT_Trace::FinalizeTargetActor()
{
	// 타겟 액터 등록 및 마무리(StartTargeting 지시)

	// 약한 포인터 가져오기...
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		// Transform을 ASC의 Avatar가 갖고 있는 Transform으로 바꿔치기할 것
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		// ASC에 추가하여 타겟 액터 등록
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);

		SpawnedTargetActor->StartTargeting(Ability);

		// 레티클 사용해서 추가 시각화(공격 범위 설정) 할 필요 없으니 바로 Confirm
		// Confirm 하게 되면 ABTA_Trace의 경우 ConfirmTargetingAndContinue 함수가 호출된다.
		// 최종 타겟 데이터가 생성되어, TargetDataReadyDelegate 델리게이트를 통해 AT에게 타겟 데이터 전달
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UABAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// true 라면, FTraceResultDelegate OnComplete 를 구독한 GA에게 알려준다

		// 타겟 액터로부터 확인이 끝났을 때, 즉 브로드캐스트를 받았을 때 AbilityTask는 종료 되어야 함
		// ( 받는 브로드캐스트 : TargetDataReadyDelegate.Broadcast(DataHandle); )
		// 종료되며 브로드캐스트 하여 GA에게 타겟 정보를 넘겨준다. (ABGA_AttackHitCheck)
		OnComplete.Broadcast(DataHandle);
	}

	// 태스크 종료 처리
	EndTask();
}
