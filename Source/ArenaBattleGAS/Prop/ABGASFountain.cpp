// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
//#include "GA/ABGA_Rotate.h"
// �����Ƽ ����� ���� �߰��ϸ� �������� ����Ƿ� �ּ� ó�� �� ��� ���� ����
#include "Tag/ABGameplayTag.h"
#include "Abilities/GameplayAbility.h"

AABGASFountain::AABGASFountain()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ActionPeriod = 3.0f;
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ������Ʈ�� �ʱ�ȭ�� ���Ŀ� �ڵ����� ���� �ʵ��� ����
	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();

	// ���Ͱ� �ʱ�ȭ�� �� �� �����Ƽ �ý��� ������Ʈ�� �Բ� �ʱ�ȭ����� ��. �׷��� �ش� �Լ� ȣ��.
	// ���� ������ ��Ÿ�� ������ �� �� this �־�����.
	// ���� ���� : ���� ASC�� �����ϰ� �����͸� �����ϴ�, ���� �۾��� �Ͼ�� �ִ� ���� ����
	// �ƹ�Ÿ ���� : ������ �����͸� ó������ ������ ���־� �������ִ� ���͸� ����
	ASC->InitAbilityActorInfo(this, this);

	// + ����
	// �����Ƽ ����� ���� �߰��߾�� �������� �������Ƿ� �߰��ߴ� ����� ���� ��,
	// ��̷� �����Ƽ ������ ����� for������ ���� ����.
	//// FGameplayAbilitySpec �� GA�� Ÿ�� ������ �Ѱ���� ��. �׷��� �̷���.
	//FGameplayAbilitySpec RotateSkillSpec(UABGA_Rotate::StaticClass());
	//ASC->GiveAbility(RotateSkillSpec);			// �����Ƽ �ߵ� �غ� �Ϸ�!

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle,
		this,							// �� ������Ʈ (AABGASFountain),
		&AABGASFountain::TimerAction,	// ������ �޼ҵ� ���ε�
		ActionPeriod,					// Ÿ�̸� �ֱ� ����
		true,							// �ݺ� (loop)
		0.0f);							// ������ ������ (�ٷ� ����ǵ���)
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin Timer"));

	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	// HasMatchingGameplayTag : �±װ� �ִ��� üũ
	// �±װ� ���� �ϸ�, �����Ƽ�� �ߵ��ǰ� �ִٰ� �ľ� ����
	// (ABTAG_ACTOR_ISROTATING �� Ȱ��ȭ�� �� �־��ִ� �±׷� ABGA_Rotate �����Ƽ �����ڿ��� �����߾����ϱ�)
	if (!ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}

	// // ����� ���� �߰��Ͽ� �ۼ��ߴ� �ڵ�
	// // ����� ������ �������� �������Ƿ� ��� �Ұ��� ��.
	// // �׷��� �ּ�ó�� �� ���� �ڵ� �ۼ�
	//FGameplayAbilitySpec* RotateGASspec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());

	//if (!RotateGASspec)
	//{
	//	ABGAS_LOG(LogABGAS, Error, TEXT("No Rotate Spec Found!"));
	//	return;
	//}

	//if (!RotateGASspec->IsActive())
	//{
	//	// ��Ȱ��ȭ �Ǿ� �ִ� �����Ƽ�� �����ؼ� Ȱ��ȭ �����ְ� ��
	//	ASC->TryActivateAbility(RotateGASspec->Handle);
	//}
	//else
	//{
	//	ASC->CancelAbilityHandle(RotateGASspec->Handle);
	//}
	
	// �����Ƽ ��� ��, ���Ϳ� ������� rotating �Լ�...
	/*if (!RotatingMovement->IsActive())
	{
		RotatingMovement->Activate(true);
	}
	else
	{
		RotatingMovement->Deactivate();
	}*/
}
