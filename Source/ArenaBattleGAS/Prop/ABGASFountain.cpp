// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
//#include "GA/ABGA_Rotate.h"
// 어빌리티 헤더를 직접 추가하면 의존성이 생기므로 주석 처리 후 기능 개선 진행
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

	// 컴포넌트가 초기화된 직후엔 자동실행 되지 않도록 끄기
	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();

	// 액터가 초기화될 때 이 어빌리티 시스템 컴포넌트도 함께 초기화해줘야 함. 그래서 해당 함수 호출.
	// 오너 정보랑 아타바 정보에 둘 다 this 넣어줬음.
	// 오너 정보 : 실제 ASC를 구동하고 데이터를 관리하는, 실제 작업이 일어나고 있는 액터 정보
	// 아바타 정보 : 실제로 데이터를 처리하진 않지만 비주얼만 수행해주는 액터를 지정
	ASC->InitAbilityActorInfo(this, this);

	// + 개선
	// 어빌리티 헤더를 직접 추가했었어서 의존성이 생겼으므로 추가했던 헤더를 삭제 후,
	// 어레이로 어빌리티 묶음을 만들어 for문으로 돌릴 예정.
	//// FGameplayAbilitySpec 로 GA의 타입 정보를 넘겨줘야 함. 그래서 이렇게.
	//FGameplayAbilitySpec RotateSkillSpec(UABGA_Rotate::StaticClass());
	//ASC->GiveAbility(RotateSkillSpec);			// 어빌리티 발동 준비 완료!

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
		this,							// 이 오브젝트 (AABGASFountain),
		&AABGASFountain::TimerAction,	// 실행할 메소드 바인딩
		ActionPeriod,					// 타이머 주기 지정
		true,							// 반복 (loop)
		0.0f);							// 딜레이 없도록 (바로 실행되도록)
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin Timer"));

	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	// HasMatchingGameplayTag : 태그가 있는지 체크
	// 태그가 존재 하면, 어빌리티가 발동되고 있다고 파악 가능
	// (ABTAG_ACTOR_ISROTATING 는 활성화될 때 넣어주는 태그로 ABGA_Rotate 어빌리티 생성자에서 지정했었으니까)
	if (!ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}

	// // 헤더를 직접 추가하여 작성했던 코드
	// // 헤더를 지워서 의존성을 없앴으므로 사용 불가가 됨.
	// // 그래서 주석처리 후 위의 코드 작성
	//FGameplayAbilitySpec* RotateGASspec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());

	//if (!RotateGASspec)
	//{
	//	ABGAS_LOG(LogABGAS, Error, TEXT("No Rotate Spec Found!"));
	//	return;
	//}

	//if (!RotateGASspec->IsActive())
	//{
	//	// 비활성화 되어 있는 어빌리티를 생성해서 활성화 시켜주게 됨
	//	ASC->TryActivateAbility(RotateGASspec->Handle);
	//}
	//else
	//{
	//	ASC->CancelAbilityHandle(RotateGASspec->Handle);
	//}
	
	// 어빌리티 사용 전, 액터에 만들었던 rotating 함수...
	/*if (!RotatingMovement->IsActive())
	{
		RotatingMovement->Activate(true);
	}
	else
	{
		RotatingMovement->Deactivate();
	}*/
}
