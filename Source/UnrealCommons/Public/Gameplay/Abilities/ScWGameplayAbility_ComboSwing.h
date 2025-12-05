// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Abilities/ScWGameplayAbility_CommonSwing.h"

#include "ScWGameplayAbility_ComboSwing.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Combo Swing Gameplay Ability"))
class UNREALCOMMONS_API UScWGameplayAbility_ComboSwing : public UScWGameplayAbility_CommonSwing
{

	GENERATED_BODY()

public:

	UScWGameplayAbility_ComboSwing();

//~ Begin Ability
protected:
	virtual void NativeActivateAbility_Commited(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UScWGameplayAbility

	UFUNCTION()
	void OnPostSwingComboWindowFinished();

	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability

//~ Begin Swing
protected:
	virtual float BP_HandlePreSwing_Implementation() override; // UScWGameplayAbility_CommonSwing
	virtual float BP_HandleEndSwing_Implementation() override; // UScWGameplayAbility_CommonSwing
	virtual void BP_HandlePostSwing_Implementation() override; // UScWGameplayAbility_CommonSwing
	
	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle SwingEndCombo"))
	void BP_HandleSwingEndComboWindow();
//~ End Swing
};
