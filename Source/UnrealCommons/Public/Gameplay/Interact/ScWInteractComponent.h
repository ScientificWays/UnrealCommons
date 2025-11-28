// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "Gameplay/ScWTypes_Gameplay.h"

#include "ScWInteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractComponentSignature, class UScWInteractComponent*, InOtherInteractComponent);

/**
 * 
 */
UCLASS(Abstract, ClassGroup = ("Interact"), meta = (DisplayName = "[ScW] Interact Component", BlueprintSpawnableComponent))
class UNREALCOMMONS_API UScWInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UScWInteractComponent();

//~ Begin Initialize
protected:
	virtual void OnRegister() override; // UActorComponent
	virtual void BeginPlay() override; // UActorComponent
//~ End Initialize

//~ Begin Data
public:

	UFUNCTION(Category = "Data", BlueprintCallable)
	const FScWInteractData& GetInteractData() const { return PrivateInteractData; }

	UFUNCTION(Category = "Data", BlueprintCallable)
	void SetInteractData(const FScWInteractData& InInteractData) { PrivateInteractData = InInteractData; }

	UFUNCTION(Category = "Data", BlueprintCallable)
	void ResetInteractData() { PrivateInteractData = FScWInteractData::Invalid; }

protected:

	UPROPERTY(Transient)
	FScWInteractData PrivateInteractData;
//~ End Data

//~ Begin State
public:

	UFUNCTION(Category = "State", BlueprintCallable)
	const bool IsInteractTarget() const { return GetInteractData().Source != nullptr; }

	UFUNCTION(Category = "State", BlueprintCallable)
	const bool IsInteractSource() const { return GetInteractData().Target != nullptr; }

	UFUNCTION(Category = "State", BlueprintCallable)
	const bool IsInInteract() const { return IsInteractTarget() || IsInteractSource(); }

	UPROPERTY(Category = "State", BlueprintAssignable)
	FInteractComponentSignature OnEmitInteractStart;

	UPROPERTY(Category = "State", BlueprintAssignable)
	FInteractComponentSignature OnEmitInteractFailed;

	UPROPERTY(Category = "State", BlueprintAssignable)
	FWasCancelledSignature OnEmitInteractFinish;

	UPROPERTY(Category = "State", BlueprintAssignable)
	FInteractComponentSignature OnReceiveInteractStart;

	UPROPERTY(Category = "State", BlueprintAssignable)
	FInteractComponentSignature OnReceiveInteractFailed;

	UPROPERTY(Category = "State", BlueprintAssignable)
	FWasCancelledSignature OnReceiveInteractFinish;

	UFUNCTION(Category = "State", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "CanReceiveInteractFrom"))
	bool BP_CanReceiveInteractFrom(UScWInteractComponent* FromInteractComponent, float& OutInteractDuration) const;
//~ End State

//~ Begin Trace
public:

	UFUNCTION(Category = "Trace", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Interact TraceStartLocation"))
	FVector BP_GetInteractTraceStartLocation() const;

	UFUNCTION(Category = "Trace", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Interact TraceDirection"))
	FVector BP_GetInteractTraceDirection() const;

	UFUNCTION(Category = "Trace", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Interact TraceMaxDistance"))
	float BP_GetInteractTraceMaxDistance() const;

	UFUNCTION(Category = "Trace", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Interact TraceWidth"))
	float BP_GetInteractTraceWidth() const;

	UFUNCTION(Category = "Trace", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Interact TraceCheckRadius"))
	float BP_GetInteractTraceCheckRadius() const;
//~ End Trace
	
//~ Begin Focus
public:
	
	UFUNCTION(Category = "Focus", BlueprintCallable)
	virtual bool CanHaveLocalInteractFocus() const { return false; }

	UFUNCTION(Category = "Focus", BlueprintCallable)
	virtual bool HasLocalInteractFocus() const { return bHasLocalInteractFocus; }

	UFUNCTION()
	virtual void SetHasLocalInteractFocus(const bool bInValue) { bHasLocalInteractFocus = bInValue; }

	UFUNCTION()
	virtual void OnReceiveLocalInteractFocus() {}

	UFUNCTION()
	virtual void OnLostLocalInteractFocus() {}
private:
	bool bHasLocalInteractFocus;
//~ End Focus
};
