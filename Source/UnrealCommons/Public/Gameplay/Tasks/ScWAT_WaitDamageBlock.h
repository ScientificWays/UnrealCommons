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

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnDamageBlocked;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitDamageBlock* WaitDamageBlock(UGameplayAbility* InOwningAbility, class UScWASC_Base* InTargetASC, TSubclassOf<UDamageType> InDamageTypeClass, const bool bOnce = true);
	
//~ Begin Common
protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Common

	UFUNCTION()
	void OnDamageBlockedCallback(float InDamage, const struct FReceiveDamageData& InData);

	FScriptDelegate OnDamageBlockedScriptDelegate;

	UPROPERTY()
	TObjectPtr<class UScWASC_Base> TargetASC;
	bool bOnce;
	TSubclassOf<UDamageType> DamageTypeClass;
};
