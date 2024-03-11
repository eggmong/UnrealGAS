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

	// Ÿ�� ���� Ŭ������ ���ڷ� �޾��� ��, �����Ƽ�½�ũ(AT)���� Ÿ�� ���͸� ���� �ϵ��� ��
	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UABAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	// ���� �ð��� ���� ��(������Ų ����) AT�� �����Ϸ���
	SetWaitingOnAvatar();
}

void UABAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		// Task�� ����� ��, ���� ������ TargetActor�� �ִٸ� ���� ó��
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UABAT_Trace::SpawnAndInitializeTargetActor()
{
	// Ÿ�� ���� ����

	SpawnedTargetActor = Cast<AABTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UABAT_Trace::OnTargetDataReadyCallback);

		// TargetActor���� ��ε�ĳ��Ʈ �ϴ� TargetDataReadyDelegate ��������Ʈ�� �����Ͽ�
		// OnTargetDataReadyCallback �Լ��� ���ε� �Ѵ�.
		// �׷��� ��ε�ĳ��Ʈ �� �� OnTargetDataReadyCallback �Լ��� ȣ��� ��.
	}
}

void UABAT_Trace::FinalizeTargetActor()
{
	// Ÿ�� ���� ��� �� ������(StartTargeting ����)

	// ���� ������ ��������...
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();

	if (ASC)
	{
		// Transform�� ASC�� Avatar�� ���� �ִ� Transform���� �ٲ�ġ���� ��
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		// ASC�� �߰��Ͽ� Ÿ�� ���� ���
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);

		SpawnedTargetActor->StartTargeting(Ability);

		// ��ƼŬ ����ؼ� �߰� �ð�ȭ(���� ���� ����) �� �ʿ� ������ �ٷ� Confirm
		// Confirm �ϰ� �Ǹ� ABTA_Trace�� ��� ConfirmTargetingAndContinue �Լ��� ȣ��ȴ�.
		// ���� Ÿ�� �����Ͱ� �����Ǿ�, TargetDataReadyDelegate ��������Ʈ�� ���� AT���� Ÿ�� ������ ����
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UABAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// true ���, FTraceResultDelegate OnComplete �� ������ GA���� �˷��ش�

		// Ÿ�� ���ͷκ��� Ȯ���� ������ ��, �� ��ε�ĳ��Ʈ�� �޾��� �� AbilityTask�� ���� �Ǿ�� ��
		// ( �޴� ��ε�ĳ��Ʈ : TargetDataReadyDelegate.Broadcast(DataHandle); )
		// ����Ǹ� ��ε�ĳ��Ʈ �Ͽ� GA���� Ÿ�� ������ �Ѱ��ش�. (ABGA_AttackHitCheck)
		OnComplete.Broadcast(DataHandle);
	}

	// �½�ũ ���� ó��
	EndTask();
}
