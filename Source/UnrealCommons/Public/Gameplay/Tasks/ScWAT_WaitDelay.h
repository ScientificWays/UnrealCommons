// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitDelay.generated.h"

/**
 *	Same as default AbilityTask_WaitDelay, but checks if provided time less or equals zero, and if it is, finishes immediately
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitDelay"))
class UNREALCOMMONS_API UScWAT_WaitDelay : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnFinish;

//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (DisplayName = "[ScW] WaitDelayOrFinishImmediately", HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitDelay* WaitDelayOrFinishImmediately(UGameplayAbility* InOwningAbility, float InTime);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize
	
//~ Begin Debug
public:
	virtual FString GetDebugString() const override; // UGameplayTask
//~ End Debug

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
