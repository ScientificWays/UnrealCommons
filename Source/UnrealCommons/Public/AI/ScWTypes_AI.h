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
	static const FName KeyTargetActor = TEXT("TargetActor");
	static const FName KeyTargetLocation = TEXT("TargetLocation");

	static const FName KeyFollowActor = TEXT("FollowActor");
	static const FName KeyInteractActor = TEXT("InteractActor");

	static const FName KeyHintActor = TEXT("HintActor");

	static const FName MeleeRangeMax = TEXT("MeleeRangeMax");

	static const FName FirearmShootDelay = TEXT("FirearmShootDelay");
	static const FName FirearmShootDuration = TEXT("FirearmShootDuration");
}

namespace FScWAIMessage
{
	static const FName AbilityEnded = TEXT("AbilityEnded");
	static const FName AbilityCancelled = TEXT("AbilityCancelled");
	static const FName SwingEnded = TEXT("SwingEnded");
}
