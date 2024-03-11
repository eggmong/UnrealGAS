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

	// AI 컨트롤러가 이 NPC 컨트롤러를 장악할 때 ASC를 초기화 하기 위해 이 함수 내부에 구현해줄 것임.
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UABCharacterAttributeSet> AttributeSet;
};
