// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/ScWTypes_AI.h"

#include "ScWBTT_MoveTo_Array.generated.h"

/**
 *	Moves the AI pawn through the specified locations array blackboard entry using the navigation system.
 */
UCLASS(meta = (DisplayName = "[ScW] BTT_MoveTo_Array"))
class UNREALCOMMONS_API UScWBTT_MoveTo_Array : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UScWBTT_MoveTo_Array();
	
//~ Begin Task
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override; // UBTNode
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTDescriptionVerbosity::Type InVerbosity, TArray<FString>& OutValues) const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
	virtual void TickTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTTaskNode
	virtual void OnTaskFinished(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, EBTNodeResult::Type InTaskResult) override; // UBTTaskNode
	EBTNodeResult::Type HandleTickWithResult(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds);
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector LocationArrayKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector CurrentArrayIndexKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	float NextPointAcceptanceRadiusXY;

	UPROPERTY(Category = "Settings", EditAnywhere)
	float MoveToAcceptanceRadiusXY;
//~ End Settings
};
