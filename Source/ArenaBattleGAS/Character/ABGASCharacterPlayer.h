// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "ABGASCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASCharacterPlayer : public AABCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASCharacterPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 플레이어가 빙의될 때 호출되는 함수
	// 여기서 PlayerStste에서 생성된 ASC를 받아올 것임.
	// 네트워크 플레이의 경우 서버에서만 호출되는 형태가 될 것이다.
	virtual void PossessedBy(AController* NewController) override;

	// 입력 처리를 위한 재정의
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void SetupGASInputComponent();
	
	// 입력 눌렀을 때
	void GASInputPressed(int32 InputId);

	// 입력이 떨어졌을 때
	void GASInputReleased(int32 InputId);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	// + 개선
	// 입력에 대한 어빌리티 목록 생성
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;
};
