// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Handhelds/ScWHandheld.h"
#include "Gameplay/Handhelds/ScWTypes_Handhelds.h"

#include "ScWHandheld_Melee.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "[ScW] Handheld (Melee)"))
class UNREALCOMMONS_API AScWHandheld_Melee : public AScWHandheld
{
	GENERATED_BODY()

public:

	AScWHandheld_Melee(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Initialize
public:

	UFUNCTION(Category = "Initialize", BlueprintCallable, meta = (KeyWords = "GetHandheldDataAsset, GetHandheldAsset, GetWeaponDataAsset, GetWeaponAsset, GetAsset"))
	class UScWHandheldData_Melee* GetMeleeDataAsset() const;

	virtual void BP_UpdateFromDataAsset_Implementation() override; // AScWHandheld
protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor
//~ End Initialize
	
//~ Begin Owner
public:
	virtual void HandleDrop() override; // AScWHandheld
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

	UFUNCTION(Category = "Patterns", BlueprintCallable)
	const FScWMeleeSwingVariantData& GetCurrentSwingVariantData() const { return CurrentSwingVariantData; }

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PreSwing"))
	void BP_PreSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "BeginSwing"))
	void BP_BeginSwing(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass);

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "EndSwing"))
	void BP_EndSwing();

protected:

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Update CurrentSwingVariantData"))
	void BP_UpdateCurrentSwingVariantData();

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

	UPROPERTY(Transient)
	FScWMeleeSwingVariantData CurrentSwingVariantData;
//~ End Swing

//~ Begin Patterns
public:

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetPatternStartLocation"))
	FVector BP_GetPatternStartLocation(const struct FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex) const;

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetNextPatternDelayTime"))
	float BP_GetNextPatternDelayTime(int32 InNextPatternIndex) const;

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "BeginTracePatterns"))
	void BP_BeginTracePatterns();

protected:

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "HandleTracePattern"))
	void BP_HandleTracePattern(const struct FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex);

	UPROPERTY(Category = "Patterns", BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> DefaultTracePatternIgnoredActorArray;

	UPROPERTY(Transient)
	FTimerHandle NextPatternDelayHandle;
//~ End Patterns

//~ Begin Debug
public:

	UPROPERTY(Category = "Debug", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDrawDebugTrace::Type> TracePatternDebugType;
//~ End Debug
};
