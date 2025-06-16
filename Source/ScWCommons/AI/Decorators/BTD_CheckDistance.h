// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "AI/Decorators/BTD_Tickable.h"

#include "BTD_CheckDistance.generated.h"

/**
 *	Check distance between two Targets and pass if less than (obviously) PassIfLessThanDistance.
 *	If either Target is not set, first key (likely SelfActor) is used.
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTD_CheckDistance : public UBTD_Tickable
{
	GENERATED_BODY()
	
public:

	UBTD_CheckDistance();
	
//~ Begin Editor
#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) override; // UObject
#endif // WITH_EDITOR	
//~ End Editor

//~ Begin Decorator
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
//~ End Decorator

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector FromTargetKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector ToTargetKey;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!PassIfLessThanDistanceKey.IsSet()"))
	float PassIfLessThanDistance;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector PassIfLessThanDistanceKey;

	UPROPERTY(Category = "Settings", EditAnywhere/*, meta = (EditCondition = "!bVerticalDistance")*/)
	bool bHorizontalDistance;

	UPROPERTY(Category = "Settings", EditAnywhere/*, meta = (EditCondition = "!bHorizontalDistance")*/)
	bool bVerticalDistance;
//~ End Settings
};
