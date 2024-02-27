// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/ABFountain.h"
#include "AbilitySystemInterface.h"
#include "ABGASFountain.generated.h"

// ���� �����Ƽ �ý���, GAS �� �߰��ϱ� ���ؼ�
// �̰��� �����ϰ� �ִ� ���ʹ� �����Ƽ �ý��� �������̽�(AbilitySystemInterface) ��� �ϴ�
// �������̽��� �������ִ� ���� ����. (�Ϲ����� ���� ���)

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASFountain();

	// IAbilitySystemInterface �� �������̽���
	// �����Ƽ �ý��� ������Ʈ(ASC)�� ��ȯ���ִ� �� �Լ��� ��������� �Ѵ�.
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	// �ֱ������� �ݺ��� Ÿ�̸� �Լ�
	virtual void TimerAction();

protected:
	// ȸ����Ű�� ���� URotatingMovementComponent ����
	UPROPERTY(VisibleAnywhere, Category=Movement)
	TObjectPtr<class URotatingMovementComponent> RotatingMovement;

	// Ÿ�̸Ӹ� �ݺ��� �ֱ� ����
	// �����Ϳ��� �� ��ĥ �� �ֵ��� EditAnywhere
	UPROPERTY(EditAnywhere, Category=Timer)
	float ActionPeriod;

	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// + ����
	// ������ �� �ߵ���ų �����Ƽ�� ���� ������ ��̷� ����
	// ��� GA���� UGameplayAbility �� ��ӹޱ� ������ TSubclassOf�� ��� ����
	UPROPERTY(EditAnywhere, Category=GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	// �𸮾� �������� �����ϴ� Ÿ�̸� �Լ� �ڵ�
	FTimerHandle ActionTimerHandle;
};
