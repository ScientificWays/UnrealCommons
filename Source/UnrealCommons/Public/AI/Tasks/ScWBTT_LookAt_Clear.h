// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/ScWTypes_AI.h"

#include "ScWBTT_LookAt_Clear.generated.h"

/**
 *	Set focal point of Owner from BlackboardKey and optional rotation offset.
 *	If key is not set, distance of 1000 cm is used.
 */
UCLASS(meta = (DisplayName = "Clear LookAt (Focus)", KeyWords = "ClearAllFocuses"))
class UNREALCOMMONS_API UScWBTT_LookAt_Clear : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UScWBTT_LookAt_Clear();
	
//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!bClearAllFocuses"))
	EATAAIFocusPriority FocusPriorityToClear;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bClearAllFocuses;
//~ End Settings
};
