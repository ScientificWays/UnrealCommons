// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitDamageBlock.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitDamageBlock"))
class UNREALCOMMONS_API UScWAT_WaitDamageBlock : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnDamageBlocked;

//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitDamageBlock* WaitDamageBlock(UGameplayAbility* InOwningAbility, class UScWASC_Base* InTargetASC, TSubclassOf<UDamageType> InDamageTypeClass, const bool bOnce = true);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Task
protected:

	UFUNCTION()
	void OnDamageBlockedCallback(float InDamage, const struct FReceivedDamageData& InData);

	FScriptDelegate OnDamageBlockedScriptDelegate;

	UPROPERTY()
	TObjectPtr<class UScWASC_Base> TargetASC;
	bool bOnce;
	TSubclassOf<UDamageType> DamageTypeClass;
//~ End Task
};
