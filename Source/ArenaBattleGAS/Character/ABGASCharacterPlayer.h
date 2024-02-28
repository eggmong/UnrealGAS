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

	// �÷��̾ ���ǵ� �� ȣ��Ǵ� �Լ�
	// ���⼭ PlayerStste���� ������ ASC�� �޾ƿ� ����.
	// ��Ʈ��ũ �÷����� ��� ���������� ȣ��Ǵ� ���°� �� ���̴�.
	virtual void PossessedBy(AController* NewController) override;

	// �Է� ó���� ���� ������
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void SetupGASInputComponent();
	
	// �Է� ������ ��
	void GASInputPressed(int32 InputId);

	// �Է��� �������� ��
	void GASInputReleased(int32 InputId);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	// + ����
	// �Է¿� ���� �����Ƽ ��� ����
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;
};
