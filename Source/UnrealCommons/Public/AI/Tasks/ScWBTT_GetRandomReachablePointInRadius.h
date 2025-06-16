// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWBTT_GetRandomReachablePointInRadius.generated.h"

/**
 * 
 */
UCLASS(meta = (KeyWords = "FindRandomPointInRadius, FindRandomReachanblePointInRadius"))
class UNREALCOMMONS_API UScWBTT_GetRandomReachablePointInRadius : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UScWBTT_GetRandomReachablePointInRadius();

//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTask
public:
	static EBTNodeResult::Type Common_Execute(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, const FBlackboardKeySelector& InCenterKey, const FBlackboardKeySelector& InRadiusKey, float InRadius, const FBlackboardKeySelector& InOutLocationKey);
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector CenterKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector RadiusKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	float Radius;
//~ End Settings
	
//~ Begin Output
protected:

	UPROPERTY(Category = "Output", EditAnywhere)
	FBlackboardKeySelector OutLocationKey;
//~ End Output
};
