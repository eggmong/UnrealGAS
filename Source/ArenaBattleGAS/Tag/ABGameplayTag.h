#pragma once

// ABGameplayTag 헤더파일만 인클루드하면 게임플레이 태그에 관련된 것들을
// 자동으로 모두 인클루드 하는 형태가 되도록 헤더 추가해줌
#include "GameplayTagContainer.h"

// define으로 만들어서 하나의 변수처럼 사용된다.
// FGameplayTag::RequestGameplayTag 를 통해 태그가 있는지 확인하고 가져올 수 있음.
#define ABTAG_ACTOR_ROTATE FGameplayTag::RequestGameplayTag(FName("Actor.Action.Rotate"))
#define ABTAG_ACTOR_ISROTATING FGameplayTag::RequestGameplayTag(FName("Actor.State.IsRotating"))