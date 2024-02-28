// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/ABGASPlayerState.h"
#include "EnhancedInputComponent.h"

AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	// �÷��̾ �����ϴ� ĳ������ ��쿣
	// ���⿡ ASC�� �����ϰ� �Ǹ�, �����͸� �����ϴ� ��ü�� 2���� ������ �Ǵ� ����.
	// (PlayerState ���� �̹� 1�� �����س���)
	
	ASC = nullptr;
	// �׷��� null �� �����ϰ�, ���� �÷��̾ ������ ��
	// PlayerState���� �����ߴ� ASC ���� ���⿡ �����ϴ� ���·� ������.

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
		// ���� ������ PlayerState��, �ƹ�Ÿ ������ this(Player) �־��־ ASC �ʱ�ȭ �Ϸ�

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

		// �÷��̾ ĳ���Ϳ� ������ �� �ܼ� Ŀ�ǵ带 ȣ���ϵ��� ��
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

		// � �Է��� ���Դ��� �ľ��ؾ� �Է¿� ���� �����Ƽ�� ������ �� ����
		// EnhancedInputComponent ��쿣 �߰����� �����͸� �Ѱ��� �� �ֵ��� ����Ǿ� ����
		// (���ε� �� �Լ��� �Ű������� �ѱ�)
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
		Spec->InputPressed = true;		// ���忡 ���� ���� ����
		if (Spec->IsActive())
		{
			// �����Ƽ�� ���� �ߵ��� �Ǿ� �ִ� ���¶��
			
			ASC->AbilitySpecInputPressed(*Spec);
			// �Է��� �Դٴ� ��ȣ�� ����
		}
		else
		{
			// �ߵ����� �ʾҴٸ� ���Ӱ� �ߵ� ��Ŵ
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
