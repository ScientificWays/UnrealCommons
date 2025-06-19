// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitInput.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitInput"))
class UNREALCOMMONS_API UScWAT_WaitInput : public UAbilityTask
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTrigger;
	
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnExpire;
	
//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitInput* WaitInputPressByInputID(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, float InExpireTime = -1.0f, const bool bCheckAlreadyPressed = false);
	
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitInput* WaitInputPressByInputID_Multi(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, int32 InPressNum = 1, float InPressMaxDelay = 0.4f, float InExpireTime = -1.0f, const bool bCheckAlreadyPressed = false);
	
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitInput* WaitInputReleaseByInputID(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, float InExpireTime = -1.0f, bool bCheckAlreadyReleased = false);
	
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitInput* WaitInputReleaseByInputID_Multi(UGameplayAbility* InOwningAbility, EScWAbilityInputID InInputID, int32 InReleaseNum = 1, float InReleaseMaxDelay = 0.4f, float InExpireTime = -1.0f, const bool bCheckAlreadyReleased = false);
	
protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize
	
//~ Begin Task
protected:

	UFUNCTION()
	void OnInputTriggerCallback(int32 InInputID);

	void PressReset();
	void ExpireCallback();

	bool bWaitMulti;
	bool bWaitRelease;

	int32 InputID;

	int32 CurrentPressNum;
	int32 TargetPressNum;
	float PressMaxDelay;

	float ExpireTime;
	bool bCheckOnActivate;
	bool bReplicateInputDirectly;

	UPROPERTY()
	TObjectPtr<class UScWASC_Base> BaseASC;

	FTimerHandle CallbackAddTimerHandle;
	FTimerHandle CurrentDelayTimerHandle;
	FTimerHandle ExpireTimerHandle;
//~ End Task
};
