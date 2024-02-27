// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/ABFountain.h"
#include "AbilitySystemInterface.h"
#include "ABGASFountain.generated.h"

// 게임 어빌리티 시스템, GAS 를 추가하기 위해선
// 이것을 소유하고 있는 액터는 어빌리티 시스템 인터페이스(AbilitySystemInterface) 라고 하는
// 인터페이스를 구현해주는 것이 좋다. (일반적인 구현 방법)

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASFountain();

	// IAbilitySystemInterface 이 인터페이스는
	// 어빌리티 시스템 컴포넌트(ASC)를 반환해주는 이 함수를 구현해줘야 한다.
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	// 주기적으로 반복할 타이머 함수
	virtual void TimerAction();

protected:
	// 회전시키기 위해 URotatingMovementComponent 선언
	UPROPERTY(VisibleAnywhere, Category=Movement)
	TObjectPtr<class URotatingMovementComponent> RotatingMovement;

	// 타이머를 반복할 주기 변수
	// 에디터에서 값 고칠 수 있도록 EditAnywhere
	UPROPERTY(EditAnywhere, Category=Timer)
	float ActionPeriod;

	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// + 개선
	// 시작할 때 발동시킬 어빌리티에 대한 정보를 어레이로 구성
	// 모든 GA들은 UGameplayAbility 를 상속받기 때문에 TSubclassOf로 어레이 생성
	UPROPERTY(EditAnywhere, Category=GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	// 언리얼 엔진에서 제공하는 타이머 함수 핸들
	FTimerHandle ActionTimerHandle;
};
