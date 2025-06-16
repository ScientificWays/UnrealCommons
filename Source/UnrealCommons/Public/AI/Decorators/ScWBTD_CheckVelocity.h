// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/Decorators/ScWBTD_Tickable.h"

#include "ScWBTD_CheckVelocity.generated.h"

/**
 *	Check velocity of Target and pass if it more than MinVelocity.
 *	If Target is not set, first key (likely SelfActor) is used.
 */
UCLASS()
class UNREALCOMMONS_API UScWBTD_CheckVelocity : public UScWBTD_Tickable
{
	GENERATED_BODY()
	
public:

	UScWBTD_CheckVelocity();

//~ Begin Decorator
protected:
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
//~ End Decorator
	
//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!MinVelocityKey.IsSet()"))
	float MinVelocity;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector MinVelocityKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bCheckHorizontalVelocityOnly;
//~ End Settings
};
