// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "ScWBTT_ClearBlackboardKey.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMMONS_API UScWBTT_ClearBlackboardKey : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UScWBTT_ClearBlackboardKey();

//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
//~ End Task

//~ Begin Settings
protected:

	
//~ End Settings
};
