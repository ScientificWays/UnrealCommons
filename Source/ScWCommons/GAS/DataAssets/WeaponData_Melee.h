// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "GAS/DataAssets/WeaponData_Base.h"

#include "WeaponData_Melee.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType)
class ATA_DREAMCOMETRUE_API UWeaponData_Melee : public UWeaponData_Base
{
	GENERATED_BODY()

public:	

	UWeaponData_Melee();
	
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
	TSubclassOf<class UATADT_Base> SwingBaseDamageTypeClass;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float PostSwingComboTimeWindow;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float SwingAIMaxRange;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	FName SwingAIMaxRange_BlackboardKeyName;
//~ End Swing
};
