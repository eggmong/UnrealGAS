// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABGASPlayerState.h"
#include "AbilitySystemComponent.h"

AABGASPlayerState::AABGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	//ASC->SetIsReplicated(true);
	// 네트워크 플레이를 감안하면 ASC는 서버에서 클라이언트로 계속 전송이 되어야 함.
	// 이를 위해 컴포넌트가 리플리케이션이 되도록 SetIsReplicated 를 true로 설정해야 함.
	// 하지만 강의에선 사용하진 않을 예정...
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
