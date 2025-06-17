// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWWeaponData_Base.generated.h"

/**
 *
 */
UCLASS(const, Abstract, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Weapon Data Base"))
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

//~ Begin Class
public:

	UPROPERTY(Category = "Class", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AScWWeapon_Base> WeaponClass;
//~ End Class
	
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
	TMap<TSubclassOf<UGameplayAbility>, int32> WeaponAbilities;
//~ End Abilities
};
