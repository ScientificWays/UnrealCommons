// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWBTS_GetRandomReachablePointInRadius.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMMONS_API UScWBTS_GetRandomReachablePointInRadius : public UBTService
{
	GENERATED_BODY()

public:

	UScWBTS_GetRandomReachablePointInRadius();
	
//~ Begin Service
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual void TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTAuxiliaryNode
//~ End Service
	
//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector CenterKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector RadiusKey;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!MinDistanceKey.IsSet()"))
	float Radius;
//~ End Settings

//~ Begin Output
	UPROPERTY(Category = "Output", EditAnywhere)
	FBlackboardKeySelector OutLocationKey;
//~ End Output
};
