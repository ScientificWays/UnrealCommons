// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWWeapon_Base.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "[ScW] Weapon (Base)"))
class AScWWeapon_Base : public AActor, public IAbilitySystemInterface
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

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Update from DataAsset"))
	void BP_UpdateFromDataAsset();

protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWWeaponData_Base> DataAsset;
//~ End Initialize
	
//~ Begin Owner
public:

	UFUNCTION(Category = "Owner", BlueprintCallable)
	class AScWCharacter* GetOwnerCharacter() const { return OwnerCharacter; }

	virtual void HandleDrop();
protected:

	UPROPERTY(Category = "Owner", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class AScWCharacter> OwnerCharacter;
//~ End Owner
	
//~ Begin Components
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

	UFUNCTION(Category = "Components", BlueprintCallable)
	UStaticMeshComponent* GetMesh() const { return Mesh; }

protected:

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
//~ End Components
};
