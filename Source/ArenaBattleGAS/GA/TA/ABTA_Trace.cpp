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
	// 액터가 존재 한다면, 핸들을 어빌리티 태스크(ABAT_Trace)로 전송하도록 브로드캐스트 함

	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);

		// ABAT_Trace 가 이 델리게이트를 구독하게 될 것임.
	}
}

FGameplayAbilityTargetDataHandle AABTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	FHitResult OutHitResult;
	const float AttackRange = 100.0f;
	const float AttackRadius = 50.0f;

	// false : 충돌 쿼리를 단순하게. true로 하면 더 많은 정보 수집
	// 자기 자신(Character)는 제외
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_Trace), false, Character);

	const FVector Forward = Character->GetActorForwardVector();

	// 캡슐의 위치로 시작점 구성 (현재 캐릭터의 위치 + 정면 방향으로 캡슐 반지름만큼 더해준 좌표값)
	const FVector Start = Character->GetActorLocation() + (Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius());
	const FVector End = Start + (Forward * AttackRange);

	// CCHANNEL_ABACTION 이걸로 채널 설정한 타겟만 검출되도록 함
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

		// 회전행렬 적용하여 캡슐 눕힘
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif

	return FGameplayAbilityTargetDataHandle();
}
