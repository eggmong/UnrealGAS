// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Attack.h"
#include "Tag/ABGameplayTag.h"
#include "Character/ABCharacterBase.h"		// npc ĳ���Ϳ��� �����Ƽ�� �߰��� ���ɼ��� �ֱ� ������
											// �θ� Ŭ������ ��� ����
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ArenaBattleGAS.h"

UABGA_Attack::UABGA_Attack()
{
	// �ν��Ͻ� ������ ����
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;


}

void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// �����Ƽ�� ����� �� ��Ÿ�� ����ϵ��� ���� �ƹ�Ÿ �������� ���� ĳ���ͷ� �� ��ȯ
	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());

	// ����� ����ϵ��� ����� ������ 'Ability Task'
	// �����Ƽ �½�ũ�� ����������, �ٷ� ���� �Ǵ� ���� �ƴϴ�.
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage());
	// OwningAbility : �� �½�ũ�� �����ϰ� �ִ� �����Ƽ
	// TaskInstanceName : �� �½�ũ�� ����ų �� �ִ� ���� �̸�
	// MontageToPlay : ĳ���Ϳ��� ���� ���� ��Ÿ�� ����

	// �½�ũ�� ����� �� ����� �Լ��� ��������Ʈ(OnCompleted)�� ���ε����ش�
	PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompleteCallback);

	// Interrupt �߻����� ���� �Լ��� ���ε�����
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UABGA_Attack::OnInterruptedCallback);

	// �̰� ȣ������� �½�ũ�� ����ȴ�.
	PlayAttackTask->ReadyForActivation();

}

void UABGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UABGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UABGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));
}

void UABGA_Attack::OnCompleteCallback()
{
	// ���ø����̼� �ɼ�. ��� ���Ұ�����...
	bool bReplicatedEndAbility = true;

	// ��ҵȰ���? -> �� �Ϸ� �ȰŴϱ� �ƴ϶� false
	bool bWasCancelled = false;

	// �����Ƽ�� �� �����ٰ� ���� ����
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
