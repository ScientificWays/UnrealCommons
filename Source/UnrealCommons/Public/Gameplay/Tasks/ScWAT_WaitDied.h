// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitDied.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitDied"))
class UNREALCOMMONS_API UScWAT_WaitDied : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnDied;
	
//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitDied* WaitDied(UGameplayAbility* InOwningAbility, class UScWASC_Base* InTargetASC);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Death
protected:

	UFUNCTION()
	void OnDiedCallback();

	UPROPERTY()
	TObjectPtr<class UScWASC_Base> TargetASC;

	FScriptDelegate OnDiedScriptDelegate;
//~ End Death
};
