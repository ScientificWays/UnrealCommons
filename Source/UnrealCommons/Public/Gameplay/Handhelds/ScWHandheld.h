// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWHandheld.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "[ScW] Handheld (Base)"))
class UNREALCOMMONS_API AScWHandheld : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AScWHandheld(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, meta = (WorldContext = "InWCO"))
	static AScWHandheld* SpawnHandheldFor(AScWCharacter* InOwner, UScWHandheldData* InData);
//~ End Statics

//~ Begin Initialize
public:

	UFUNCTION(Category = "Initialize", BlueprintCallable)
	class UScWHandheldData* GetDataAsset() const { return DataAsset; }

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Update from DataAsset"))
	void BP_UpdateFromDataAsset();

protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWHandheldData> DataAsset;
//~ End Initialize
	
//~ Begin Owner
public:

	UFUNCTION(Category = "Owner", BlueprintCallable)
	class AScWCharacter* GetOwnerCharacter() const { return OwnerCharacter; }

	virtual void UpdateAttachmentToOwner();
	virtual void HandleDrop();
protected:

	UPROPERTY(Transient)
	TObjectPtr<class AScWCharacter> OwnerCharacter;
//~ End Owner
	
//~ Begin Components
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

	UFUNCTION(Category = "Components", BlueprintCallable)
	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(Category = "Components", BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent; }

	UFUNCTION(Category = "Components", BlueprintCallable)
	UMeshComponent* GetRelevantMeshComponent() const;

	UFUNCTION(Category = "Handheld", BlueprintCallable, meta = (KeyWords = "GetAnimInstance", DisplayName = "Get Anim Instance (Mesh)"))
	class UScWAnimInstance_Handheld* GetMeshAnimInstance() const;

protected:

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
//~ End Components
};
