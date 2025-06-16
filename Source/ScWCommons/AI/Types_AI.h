// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "Types_AI.generated.h"

UENUM(BlueprintType)
enum class EATAAIFocusPriority : uint8
{
	Default = 0u,
	Move = 1u,
	Gameplay1 = 2u,
	Gameplay2 = 3u,
	Gameplay3 = 4u
};

namespace FATABlackboard
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
