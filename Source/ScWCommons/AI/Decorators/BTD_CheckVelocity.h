// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "AI/Decorators/BTD_Tickable.h"

#include "BTD_CheckVelocity.generated.h"

/**
 *	Check velocity of Target and pass if it more than MinVelocity.
 *	If Target is not set, first key (likely SelfActor) is used.
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTD_CheckVelocity : public UBTD_Tickable
{
	GENERATED_BODY()
	
public:

	UBTD_CheckVelocity();

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
