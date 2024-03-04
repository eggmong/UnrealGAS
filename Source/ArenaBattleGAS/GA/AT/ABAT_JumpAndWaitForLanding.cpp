// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_JumpAndWaitForLanding.h"
#include "Gameframework/Character.h"

UABAT_JumpAndWaitForLanding::UABAT_JumpAndWaitForLanding()
{
}

UABAT_JumpAndWaitForLanding* UABAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UABAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UABAT_JumpAndWaitForLanding>(OwningAbility);

	return NewTask;
}

void UABAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	
	// ACharacter 에 존재하는, 땅에 닿았을 때 호출되는 델리게이트 함수
	Character->LandedDelegate.AddDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	// 점프가 언제 끝날지 모르기 때문에 이 함수를 호출해서
	// Task의 상태를 Waiting 으로 설정해줌
	SetWaitingOnAvatar();
}

void UABAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());

	Character->LandedDelegate.RemoveDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UABAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// true 라면, FJumpAndWaitForLandingDelegate OnComplete 를 구독한 GA에게 알려준다

		OnComplete.Broadcast();
	}
}
