// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWWeapon_Base.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class AScWWeapon_Base : public AActor
{
	GENERATED_BODY()

public:

	AScWWeapon_Base(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, meta = (WorldContext = "InWCO"))
	static AScWWeapon_Base* SpawnWeapon(AScWCharacter* InOwner, UScWWeaponData_Base* InData);
//~ End Statics

//~ Begin Initialize
public:

	UFUNCTION(Category = "Initialize", BlueprintCallable)
	class UScWWeaponData_Base* GetDataAsset() const { return DataAsset; }

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, BlueprintCallable)
	void K2_UpdateFromDataAsset();

protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWWeaponData_Base> DataAsset;

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class AScWCharacter> OwnerCharacter;
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
