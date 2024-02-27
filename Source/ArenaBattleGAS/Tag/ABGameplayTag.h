#pragma once

// ABGameplayTag ������ϸ� ��Ŭ����ϸ� �����÷��� �±׿� ���õ� �͵���
// �ڵ����� ��� ��Ŭ��� �ϴ� ���°� �ǵ��� ��� �߰�����
#include "GameplayTagContainer.h"

// define���� ���� �ϳ��� ����ó�� ���ȴ�.
// FGameplayTag::RequestGameplayTag �� ���� �±װ� �ִ��� Ȯ���ϰ� ������ �� ����.
#define ABTAG_ACTOR_ROTATE FGameplayTag::RequestGameplayTag(FName("Actor.Action.Rotate"))
#define ABTAG_ACTOR_ISROTATING FGameplayTag::RequestGameplayTag(FName("Actor.State.IsRotating"))