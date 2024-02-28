// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABGASPlayerState.h"
#include "AbilitySystemComponent.h"

AABGASPlayerState::AABGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	//ASC->SetIsReplicated(true);
	// ��Ʈ��ũ �÷��̸� �����ϸ� ASC�� �������� Ŭ���̾�Ʈ�� ��� ������ �Ǿ�� ��.
	// �̸� ���� ������Ʈ�� ���ø����̼��� �ǵ��� SetIsReplicated �� true�� �����ؾ� ��.
	// ������ ���ǿ��� ������� ���� ����...
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
