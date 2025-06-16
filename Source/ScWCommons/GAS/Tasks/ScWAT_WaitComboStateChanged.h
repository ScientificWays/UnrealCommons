// Scientific Ways

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAAT_WaitComboStateChanged.generated.h"

/**
 *
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UATAAT_WaitComboStateChanged : public UAbilityTask
{
	GENERATED_BODY()

public:

	UATAAT_WaitComboStateChanged();

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnComboStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UATAAT_WaitComboStateChanged* WaitComboStateChanged(UGameplayAbility* InOwningAbility);
	
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
