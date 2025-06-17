// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/Decorators/ScWBTD_Tickable.h"

#include "ScWBTD_CanActivateAbility.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] BTD_CanActivateAbility"))
class UNREALCOMMONS_API UScWBTD_CanActivateAbility : public UScWBTD_Tickable
{
	GENERATED_BODY()

	/*struct FThisTaskMemory
	{
		FGameplayAbilitySpec* AbilitySpec;
	};*/

public:

	UScWBTD_CanActivateAbility();

//~ Begin Decorator
protected:
	//virtual uint16 GetInstanceMemorySize() const override; // UBTNode
	virtual FString GetStaticDescription() const override; // UBTNode
	//virtual void OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
	FGameplayAbilitySpec* TryGetAbilitySpec(const UBehaviorTreeComponent& InOwnerTree, const UAbilitySystemComponent* FromASC) const;
//~ End Decorator

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "AbilityClassKey != nullptr"))
	FGameplayTagContainer AbilityTags;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector AbilityClassKey; // If set, try find ability by class

	//UPROPERTY(Category = "Settings", EditAnywhere)
	//bool bTryCacheAbilityOnBecomeRelevant;
//~ End Settings
};
