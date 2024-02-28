// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/ABGASPlayerState.h"
#include "EnhancedInputComponent.h"

AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	// 플레이어가 빙의하는 캐릭터의 경우엔
	// 여기에 ASC를 생성하게 되면, 데이터를 관리하는 주체가 2개가 생성이 되는 거임.
	// (PlayerState 에서 이미 1개 생성해놨음)
	
	ASC = nullptr;
	// 그래서 null 로 설정하고, 실제 플레이어가 빙의할 때
	// PlayerState에서 생성했던 ASC 값을 여기에 대입하는 형태로 구현함.

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattleGAS/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}
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

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();

		// 플레이어가 캐릭터에 빙의할 때 콘솔 커맨드를 호출하도록 함
		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}


}

void AABGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();
}

void AABGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		// 어떤 입력이 들어왔는지 파악해야 입력에 따른 어빌리티를 실행할 수 있음
		// EnhancedInputComponent 경우엔 추가적인 데이터를 넘겨줄 수 있도록 설계되어 있음
		// (바인드 할 함수의 매개변수로 넘김)
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AABGASCharacterPlayer::GASInputReleased, 0);
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 1);
	}
}

void AABGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;		// 스펙에 현재 상태 지정
		if (Spec->IsActive())
		{
			// 어빌리티가 현재 발동이 되어 있는 상태라면
			
			ASC->AbilitySpecInputPressed(*Spec);
			// 입력이 왔다는 신호를 전달
		}
		else
		{
			// 발동하지 않았다면 새롭게 발동 시킴
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AABGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}
