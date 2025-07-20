// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWTypes_Gameplay.h"

#include "ScWWeaponData_Base.generated.h"

/**
 *
 */
UCLASS(const, Abstract, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Weapon Data (Base)"))
class UNREALCOMMONS_API UScWWeaponData_Base : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWWeaponData_Base();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
//~ End UI

//~ Begin General
public:

	UPROPERTY(Category = "General", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AScWWeapon_Base> WeaponClass;

	UPROPERTY(Category = "General", EditDefaultsOnly, BlueprintReadOnly)
	bool bCanDrop;

	UPROPERTY(Category = "General", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bCanDrop"))
	bool bDropOnDeath;
//~ End General
	
//~ Begin Mesh
public:

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	FName OwnerMeshAttachmentSocket;
//~ End Mesh

//~ Begin Animations
public:

	//UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<UAnimInstance> AnimInstanceClass;
//~ End Animations
	
//~ Begin Abilities
public:

	UPROPERTY(Category = "Abilities", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWGameplayGiveAbilityData> WeaponAbilitiesGiveData;
//~ End Abilities
};
