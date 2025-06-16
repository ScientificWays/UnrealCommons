// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitComboStateChanged.generated.h"

/**
 *
 */
UCLASS()
class UNREALCOMMONS_API UScWAT_WaitComboStateChanged : public UAbilityTask
{
	GENERATED_BODY()

public:

	UScWAT_WaitComboStateChanged();

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnComboStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitComboStateChanged* WaitComboStateChanged(UGameplayAbility* InOwningAbility);
	
//~ Begin Initialize
protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Callbacks
protected:

	UFUNCTION()
	void OnComboStateChangedCallback();

	FScriptDelegate CallbackScriptDelegate;
//~ End Callbacks
};
