// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Handhelds/ScWHandheldData.h"

#include "ScWHandheldData_Melee.generated.h"

//DECLARE_DELEGATE_RetVal_OneParam(bool, FScWMeleeSwingPatternFireEvent, int32);

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Melee Swing Pattern Data"))
struct FScWMeleeSwingPatternData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TraceOffsetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TraceOffsetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceLength = 128.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceShapeRadius = 32.0f;
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
	FVector2D CapsuleRadiusHeight;

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly)
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

//~ Begin Patterns
public:

	//UFUNCTION(Category = "Patterns", BlueprintCallable)
	//void BeginFireTraces(FScWMeleeSwingPatternFireEvent InPatternFireEvent);

	UPROPERTY(Category = "Patterns", EditDefaultsOnly, BlueprintReadOnly)
	bool bIsUsingPatterns;

	UPROPERTY(Category = "Patterns", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingPatterns"))
	TArray<FScWMeleeSwingPatternData> Patterns;

	UPROPERTY(Category = "Patterns", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingPatterns"))
	float PatternsTriggerTime;

	UFUNCTION(Category = "Patterns", BlueprintCallable)
	float GetNextPatternDelayTime(int32 InNextPatternIndex) const;

//protected:
	//void HandlePatternFire();
//~ End Patterns

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SwingMontage;
//~ End Animations
};
