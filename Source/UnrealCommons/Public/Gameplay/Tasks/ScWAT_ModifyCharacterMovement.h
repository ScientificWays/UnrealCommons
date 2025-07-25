// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_ModifyCharacterMovement.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_ModifyCharacterMovement"))
class UNREALCOMMONS_API UScWAT_ModifyCharacterMovement : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Time Out"))
	FGenericGameplayTaskDelegate OnTimeOutExec;

//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_ModifyCharacterMovement* ModifyCharacterMovement(UGameplayAbility* InOwningAbility, class UScWCharacterMovement* InTarget, FVector InMagnitude, const bool bInApplyAsForce = false, float InTimeOut = -1.0f);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Tick
protected:
	virtual void TickTask(float InDeltaSeconds) override; // UGameplayTask
//~ End Tick

//~ Begin TimeOut
protected:
	void OnTimeOut();
//~ End TimeOut

	FVector Magnitude;
	bool bApplyAsForce;

	FTimerHandle TimeOutTimerHandle;
	float TimeOut;

	UPROPERTY()
	TObjectPtr<class UScWCharacterMovement> TargetCMC;
};
