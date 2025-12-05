// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Handhelds/ScWHandheld.h"
#include "Gameplay/Handhelds/ScWTypes_Handhelds.h"

#include "ScWHandheld_Melee.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWMeleeHandheldPreSwingSignature, float, InPreSwingDelay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWMeleeHandheldBeginSwingSignature, float, InSwingDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScWMeleeHandheldEndSwingSignature, float, InEndSwingDelay, const bool, bInWasCancelled);

UENUM(BlueprintType, meta = (DisplayName = "[ScW] Ability Input ID"))
enum class EScWSwingPhase : uint8
{
	None,
	PreSwing,
	Swing,
};

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

	UFUNCTION(Category = "Swing", BlueprintCallable)
	EScWSwingPhase GetCurrentSwingPhase() const { return CurrentSwingPhase; }

	UFUNCTION(Category = "Swing", BlueprintCallable)
	const FScWMeleeSwingVariantData& GetCurrentSwingVariantData() const { return CurrentSwingVariantData; }

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get SwingDamage"))
	float BP_GetSwingDamage() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get SwingDamageTypeClass"))
	TSubclassOf<class UScWDamageType> BP_GetSwingDamageTypeClass() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "PreSwing"))
	float BP_PreSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "BeginSwing"))
	float BP_BeginSwing(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass);

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "EndSwing"))
	float BP_EndSwing(const bool bInWasCancelled);

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "ResetSwingComponents"))
	void BP_ResetSwingComponents();

	UPROPERTY(Category = "Swing", BlueprintAssignable)
	FScWMeleeHandheldPreSwingSignature OnPreSwingDelegate;

	UPROPERTY(Category = "Swing", BlueprintAssignable)
	FScWMeleeHandheldBeginSwingSignature OnBeginSwingDelegate;

	UPROPERTY(Category = "Swing", BlueprintAssignable)
	FScWMeleeHandheldEndSwingSignature OnEndSwingDelegate;

protected:

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Update CurrentSwingVariantData"))
	void BP_UpdateCurrentSwingVariantData();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "HandleSwingHit"))
	bool BP_HandleSwingHit(AActor* InHitActor, const FHitResult& InHitResult);

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	int32 SwingCounter;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	EScWSwingPhase CurrentSwingPhase;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	float LastSwingDamage;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	TSubclassOf<UDamageType> LastSwingDamageTypeClass;

	UPROPERTY(Category = "Swing", BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> LastSwingAffectedActorArray;

	UPROPERTY(Category = "Swing", EditAnywhere, BlueprintReadWrite)
	int32 PreSwingMontageSectionIndex;

	UPROPERTY(Category = "Swing", EditAnywhere, BlueprintReadWrite)
	int32 SwingMontageSectionIndex;

	UPROPERTY(Category = "Swing", EditAnywhere, BlueprintReadWrite)
	int32 EndSwingMontageSectionIndex;

	UPROPERTY(Transient)
	FScWMeleeSwingVariantData CurrentSwingVariantData;
//~ End Swing

//~ Begin Swing FX
protected:

	UPROPERTY(Category = "Swing FX", Transient, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> SwingPaticlesComponent;
//~ End Swing FX

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
	TSet<TObjectPtr<AActor>> SwingHitIgnoredActors;

	UPROPERTY(Category = "Patterns", BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> TracePatternIgnoredActors;

	UPROPERTY(Transient)
	FTimerHandle NextPatternDelayHandle;
//~ End Patterns

//~ Begin Debug
public:

	UPROPERTY(Category = "Debug", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDrawDebugTrace::Type> TracePatternDebugType;
//~ End Debug
};
