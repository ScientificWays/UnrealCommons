// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "WeaponData_Base.generated.h"

/**
 *
 */
UCLASS(const, Abstract, Blueprintable, BlueprintType)
class ATA_DREAMCOMETRUE_API UWeaponData_Base : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UWeaponData_Base();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
//~ End UI

//~ Begin Class
public:

	UPROPERTY(Category = "Class", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AATAWeapon_Base> WeaponClass;
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
