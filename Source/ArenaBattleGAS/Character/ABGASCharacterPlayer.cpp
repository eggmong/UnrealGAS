// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/ABGASPlayerState.h"

AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	// 플레이어가 빙의하는 캐릭터의 경우엔
	// 여기에 ASC를 생성하게 되면, 데이터를 관리하는 주체가 2개가 생성이 되는 거임.
	// (PlayerState 에서 이미 1개 생성해놨음)
	
	ASC = nullptr;
	// 그래서 null 로 설정하고, 실제 플레이어가 빙의할 때
	// PlayerState에서 생성했던 ASC 값을 여기에 대입하는 형태로 구현함.
}

UAbilitySystemComponent* AABGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AABGASPlayerState* GASPS = GetPlayerState<AABGASPlayerState>();

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);
		// 오너 정보에 PlayerState을, 아바타 정보엔 this(Player) 넣어주어서 ASC 초기화 완료

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}
	}


}
