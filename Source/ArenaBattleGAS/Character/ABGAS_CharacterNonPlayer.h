// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterNonPlayer.h"
#include "AbilitySystemInterface.h"
#include "ABGAS_CharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGAS_CharacterNonPlayer : public AABCharacterNonPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGAS_CharacterNonPlayer();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// AI ��Ʈ�ѷ��� �� NPC ��Ʈ�ѷ��� ����� �� ASC�� �ʱ�ȭ �ϱ� ���� �� �Լ� ���ο� �������� ����.
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UABCharacterAttributeSet> AttributeSet;
};
