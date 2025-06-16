// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ComboData.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType)
class ATA_DREAMCOMETRUE_API UComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UComboData();

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
	TArray<TObjectPtr<class UComboMoveData>> Moves;
//~ End Moves

//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "ModifyWeaponDamage"))
	float K2_ModifyWeaponDamage(const class UWeaponData_Base* InWeaponData, float InDamage) const;

	UFUNCTION(Category = "Damage", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "ModifyWeaponDamageTypeClass"))
	TSubclassOf<class UATADT_Base> K2_ModifyWeaponDamageTypeClass(const class UWeaponData_Base* InWeaponData, TSubclassOf<class UATADT_Base> InDamageTypeClass) const;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageMul;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	float DamageAdditive;

	UPROPERTY(Category = "Damage", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UATADT_Base> OverrideDamageTypeClass;
//~ End Damage

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> MontageVariants;
//~ End Animations
};
