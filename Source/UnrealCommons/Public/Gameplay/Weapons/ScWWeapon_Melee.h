// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Weapons/ScWWeapon_Base.h"

#include "ScWWeapon_Melee.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "[ScW] Weapon (Melee)"))
class UNREALCOMMONS_API AScWWeapon_Melee : public AScWWeapon_Base
{
	GENERATED_BODY()

public:

	AScWWeapon_Melee(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Initialize
public:

	UFUNCTION(Category = "Initialize", BlueprintCallable, meta = (KeyWords = "GetWeaponDataAsset, GetWeaponAsset, GetAsset"))
	class UScWWeaponData_Melee* GetMeleeDataAsset() const;

	virtual void BP_UpdateFromDataAsset_Implementation() override; // AScWWeapon_Base
protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor
//~ End Initialize
	
//~ Begin Owner
public:
	virtual void HandleDrop() override; // AScWWeapon_Base
//~ End Owner

//~ Begin Components
public:

	UFUNCTION(Category = "Components", BlueprintCallable)
	UCapsuleComponent* GetCollisionComponent() const { return CollisionComponent; }

protected:

	UFUNCTION()
	void OnCollisionComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepHitResult);

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> CollisionComponent;
//~ End Components

//~ Begin Swing
public:

	UFUNCTION(Category = "Swing", BlueprintCallable)
	int32 GetSwingCounter() const { return SwingCounter; }

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "BeginSwing"))
	void BP_BeginSwing(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass);

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "EndSwing"))
	void BP_EndSwing();

protected:

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "HandleSwingHit"))
	void BP_HandleSwingHit(const FHitResult& InHitResult);

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	int32 SwingCounter;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	float LastSwingDamage;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	TSubclassOf<UDamageType> LastSwingDamageTypeClass;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> LastSwingAffectedActorArray;
//~ End Swing

//~ Begin Patterns
public:

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetPatternStartLocation"))
	FVector BP_GetPatternStartLocation(const struct FScWMeleeSwingPatternData& InPatternData, int32 InPatternIndex) const;

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "BeginPatternTraces"))
	void BP_BeginPatternTraces();

protected:

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "HandlePatternTrace"))
	void BP_HandlePatternTrace(const struct FScWMeleeSwingPatternData& InPatternData, int32 InPatternIndex);

	UPROPERTY(Category = "Patterns", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDrawDebugTrace::Type> PatternTraceDebugType;

	UPROPERTY(Category = "Patterns", BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> DefaultPatternTraceIgnoredActorArray;

	UPROPERTY()
	FTimerHandle NextPatternDelayHandle;
//~ End Patterns
};
