// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/DataAssets/ScWWeaponData_Base.h"

#include "ScWWeaponData_Melee.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType)
class UNREALCOMMONS_API UScWWeaponData_Melee : public UScWWeaponData_Base
{
	GENERATED_BODY()

public:	

	UScWWeaponData_Melee();
	
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
};
