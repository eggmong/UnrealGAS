// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "Gameframework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"

AABTA_Trace::AABTA_Trace()
{
}

void AABTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AABTA_Trace::ConfirmTargetingAndContinue()
{
	// ���Ͱ� ���� �Ѵٸ�, �ڵ��� �����Ƽ �½�ũ(ABAT_Trace)�� �����ϵ��� ��ε�ĳ��Ʈ ��

	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);

		// ABAT_Trace �� �� ��������Ʈ�� �����ϰ� �� ����.
	}
}

FGameplayAbilityTargetDataHandle AABTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	FHitResult OutHitResult;
	const float AttackRange = 100.0f;
	const float AttackRadius = 50.0f;

	// false : �浹 ������ �ܼ��ϰ�. true�� �ϸ� �� ���� ���� ����
	// �ڱ� �ڽ�(Character)�� ����
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_Trace), false, Character);

	const FVector Forward = Character->GetActorForwardVector();

	// ĸ���� ��ġ�� ������ ���� (���� ĳ������ ��ġ + ���� �������� ĸ�� ��������ŭ ������ ��ǥ��)
	const FVector Start = Character->GetActorLocation() + (Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius());
	const FVector End = Start + (Forward * AttackRange);

	// CCHANNEL_ABACTION �̰ɷ� ä�� ������ Ÿ�ٸ� ����ǵ��� ��
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		// ȸ����� �����Ͽ� ĸ�� ����
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif

	return FGameplayAbilityTargetDataHandle();
}
