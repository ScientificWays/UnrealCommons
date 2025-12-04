// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWComboMoveData.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Combo Move Data"))
class UNREALCOMMONS_API UScWComboMoveData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWComboMoveData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image;
//~ End UI
	
//~ Begin Power
public:

	UPROPERTY(Category = "Power", EditDefaultsOnly, BlueprintReadOnly)
	float MovePowerMul;
//~ End Power
	
//~ Begin Damage
public:

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float MoveDamageMul;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float MoveDamageAdditive;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWDamageType> OverrideMoveDamageTypeClass;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float MoveDamageImpactImpulseMul;
//~ End Damage
};
