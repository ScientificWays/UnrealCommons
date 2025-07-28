// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWTypes_Animations.h"

#include "ScWTypes_Handhelds.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Melee Swing Variant Data: Trace Pattern"))
struct FScWMeleeSwingVariantData_TracePattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsUsingTraces"))
	FRotator TraceOffsetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsUsingTraces"))
	FVector TraceOffsetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsUsingTraces"))
	float TraceLength = 128.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsUsingTraces"))
	float TraceShapeRadius = 32.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Melee Swing Variant Data"))
struct FScWMeleeSwingVariantData
{
	GENERATED_BODY()

	// Can be empty if no traces needed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FScWMeleeSwingVariantData_TracePattern> TracePatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScWCharacterMontageData MontageData;

	static const FScWMeleeSwingVariantData Invalid;
};
