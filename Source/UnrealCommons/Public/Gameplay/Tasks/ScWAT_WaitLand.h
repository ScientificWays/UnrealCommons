// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitLand.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitLand"))
class UNREALCOMMONS_API UScWAT_WaitLand : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnPlayerLanded;
	
//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitLand* WaitLand(UGameplayAbility* InOwningAbility);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

	UFUNCTION()
	void OnPlayerLandedCallback(const FHitResult& InHit);

	FScriptDelegate LandedDelegate;
};
