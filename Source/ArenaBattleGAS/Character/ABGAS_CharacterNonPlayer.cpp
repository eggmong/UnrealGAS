// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGAS_CharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"

AABGAS_CharacterNonPlayer::AABGAS_CharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AABGAS_CharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGAS_CharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// npc는 PlayerStat 을 사용하지 않아서 모두 자신(this)를 넣어주게됨.
	ASC->InitAbilityActorInfo(this, this);
}
