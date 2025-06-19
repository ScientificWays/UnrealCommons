// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAT_WaitInput_Move.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AT_WaitInput_Move"))
class UNREALCOMMONS_API UScWAT_WaitInput_Move : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(Category = "Callbacks", BlueprintAssignable)
	FGenericGameplayTaskDelegate OnInputTriggered;
	
//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitInput_Move* WaitInput_Move(UGameplayAbility* InOwningAbility, class AScWCharacter* InTargetCharacter, bool bTriggerOnForward = true, bool bTriggerOnBackward = true, bool bTriggerOnRight = true, bool bTriggerOnLeft = true, bool bOnce = true);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Task
protected:

	UPROPERTY()
	TObjectPtr<class AScWCharacter> TargetCharacter;

	bool bTriggerOnForward;
	bool bTriggerOnBackward;
	bool bTriggerOnRight;
	bool bTriggerOnLeft;

	bool bOnce;

	UFUNCTION()
	void OnInputTriggeredCallback();

	FScriptDelegate InputTriggeredDelegate;
//~ End Task
};
