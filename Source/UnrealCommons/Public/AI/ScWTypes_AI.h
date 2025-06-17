// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_AI.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "[ScW] AI Focus Priority"))
enum class EScWAIFocusPriority : uint8
{
	Default = 0u,
	Move = 1u,
	Gameplay1 = 2u,
	Gameplay2 = 3u,
	Gameplay3 = 4u
};

namespace FScWBlackboard
{
	const FName KeyTargetActor = TEXT("TargetActor");
	const FName KeyTargetLocation = TEXT("TargetLocation");

	const FName KeyFollowActor = TEXT("FollowActor");
	const FName KeyInteractActor = TEXT("InteractActor");

	const FName KeyHintActor = TEXT("HintActor");

	const FName MeleeRangeMax = TEXT("MeleeRangeMax");

	const FName FirearmShootDelay = TEXT("FirearmShootDelay");
	const FName FirearmShootDuration = TEXT("FirearmShootDuration");
}
