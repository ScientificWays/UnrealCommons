// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Handhelds/ScWHandheldData.h"
#include "Gameplay/Handhelds/ScWTypes_Handhelds.h"

#include "ScWHandheldData_Melee.generated.h"

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

//~ Begin Swing Variants
public:

	UPROPERTY(Category = "Swing Variants", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWMeleeSwingVariantData> SwingVariants;

	UPROPERTY(Category = "Swing Variants", EditDefaultsOnly, BlueprintReadOnly)
	float SwingVariantBaseDuration;
//~ End Swing Variants
};
