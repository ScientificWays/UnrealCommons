// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAWeapon_Base.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class AATAWeapon_Base : public AActor
{
	GENERATED_BODY()

public:

	AATAWeapon_Base(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, meta = (WorldContext = "InWCO"))
	static AATAWeapon_Base* SpawnWeapon(AATACharacter* InOwner, UWeaponData_Base* InData);
//~ End Statics

//~ Begin Initialize
public:

	UFUNCTION(Category = "Initialize", BlueprintCallable)
	class UWeaponData_Base* GetDataAsset() const { return DataAsset; }

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, BlueprintCallable)
	void K2_UpdateFromDataAsset();

protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWeaponData_Base> DataAsset;

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class AATACharacter> OwnerCharacter;
//~ End Initialize
	
//~ Begin Components
public:

	UFUNCTION(Category = "Components", BlueprintCallable)
	UStaticMeshComponent* GetMesh() const { return Mesh; }

protected:

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
//~ End Components
};
