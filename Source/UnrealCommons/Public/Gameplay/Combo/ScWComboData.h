// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWComboData.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Combo Data"))
class UNREALCOMMONS_API UScWComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWComboData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image;
//~ End UI

//~ Begin Moves
public:

	UPROPERTY(Category = "Moves", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<const class UScWComboMoveData>> Moves;
//~ End Moves

//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "ModifyHandheldDamage"))
	float BP_ModifyHandheldDamage(const class UScWHandheldData* InHandheldData, float InDamage) const;

	UFUNCTION(Category = "Damage", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "ModifyHandheldDamageTypeClass"))
	TSubclassOf<class UScWDamageType> BP_ModifyHandheldDamageTypeClass(const class UScWHandheldData* InHandheldData, TSubclassOf<class UScWDamageType> InDamageTypeClass) const;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageMul;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageAdditive;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWDamageType> OverrideDamageTypeClass;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageImpactImpulseMul;
//~ End Damage

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<const UAnimMontage>> MontageVariants;
//~ End Animations
};
