// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWTypes_Animations.h"

#include "Gameplay/Handhelds/ScWHandheldData.h"

#include "ScWHandheldData_Melee.generated.h"

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

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Handheld Data (Melee)"))
class UNREALCOMMONS_API UScWHandheldData_Melee : public UScWHandheldData
{
	GENERATED_BODY()

public:	

	UScWHandheldData_Melee();
	
//~ Begin Physics
public:

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly)
	bool bIsUsingCollisionComponent;

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingCollisionComponent"))
	FVector2D CapsuleRadiusHeight;

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingCollisionComponent"))
	FTransform CapsuleRelativeTransform;
//~ End Physics
	
//~ Begin Swing
public:

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float SwingBaseDamage;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWDamageType> SwingBaseDamageTypeClass;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float PostSwingComboTimeWindow;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float SwingAIMaxRange;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	FName SwingAIMaxRange_BlackboardKeyName;
//~ End Swing

//~ Begin Variants
public:

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetNewSwingVariantIndexFor"))
	int32 BP_GetNewSwingVariantIndexFor(const class AScWHandheld_Melee* InMeleeHandheld) const;

	UPROPERTY(Category = "Variants", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWMeleeSwingVariantData> Variants;

	UPROPERTY(Category = "Variants", EditDefaultsOnly, BlueprintReadOnly)
	float VariantBaseDuration;

	UFUNCTION(Category = "Variants", BlueprintCallable)
	float GetNextPatternDelayTime(int32 InVariantIndex, int32 InNextPatternIndex) const;
//~ End Variants
};
