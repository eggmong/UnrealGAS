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

	// npc�� PlayerStat �� ������� �ʾƼ� ��� �ڽ�(this)�� �־��ְԵ�.
	ASC->InitAbilityActorInfo(this, this);
}
