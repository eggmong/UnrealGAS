// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/ABGASPlayerState.h"

AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	// �÷��̾ �����ϴ� ĳ������ ��쿣
	// ���⿡ ASC�� �����ϰ� �Ǹ�, �����͸� �����ϴ� ��ü�� 2���� ������ �Ǵ� ����.
	// (PlayerState ���� �̹� 1�� �����س���)
	
	ASC = nullptr;
	// �׷��� null �� �����ϰ�, ���� �÷��̾ ������ ��
	// PlayerState���� �����ߴ� ASC ���� ���⿡ �����ϴ� ���·� ������.
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
		// ���� ������ PlayerState��, �ƹ�Ÿ ������ this(Player) �־��־ ASC �ʱ�ȭ �Ϸ�

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}
	}


}
