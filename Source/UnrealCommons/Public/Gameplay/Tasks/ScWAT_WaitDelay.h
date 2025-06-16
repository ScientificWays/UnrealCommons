// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitDelay.generated.h"

/**
 *	Same as default AbilityTask_WaitDelay, but checks if provided time less or equals zero, and if it is, finishes immediately
 */
UCLASS()
class UNREALCOMMONS_API UScWAT_WaitDelay : public UAbilityTask
{
	GENERATED_BODY()

public:

	UScWAT_WaitDelay();

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnFinish;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitDelay* WaitDelayOrFinishImmediately(UGameplayAbility* InOwningAbility, float InTime);
	
//~ Begin Debug
public:
	virtual FString GetDebugString() const override; // UGameplayTask
//~ End Debug

//~ Begin Initialize
protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Callbacks
protected:
	void OnTimeFinish();
//~ End Callbacks

//~ Begin Data
protected:
	FTimerHandle TimerHandle;
	float Time;
	float TimeStarted;
//~ End Data
};
