// Scientific Ways

#include "AI/Tasks/ScWBTT_LookAt_Clear.h"

#include "AI/ScWTypes_AI.h"
#include "AI/ScWAIController.h"

UScWBTT_LookAt_Clear::UScWBTT_LookAt_Clear()
{
	NodeName = "Clear Look At";

	FocusPriorityToClear = EScWAIFocusPriority::Gameplay3;
	bClearAllFocuses = false;
}

//~ Begin Task
FString UScWBTT_LookAt_Clear::GetStaticDescription() const // UBTNode
{
	static const UEnum* FocusPriorityEnum = StaticEnum<EScWAIFocusPriority>();
	FString FocusPriorityName = FocusPriorityEnum->GetNameStringByIndex(static_cast<int32>(FocusPriorityToClear));
	return bClearAllFocuses ? TEXT("Clear all focuses") : FString::Printf(TEXT("Clear %s focus"), *FocusPriorityName);
}

EBTNodeResult::Type UScWBTT_LookAt_Clear::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	if (AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner()))
	{
		if (bClearAllFocuses)
		{
			OwnerController->ClearAllFocuses();
		}
		else
		{
			OwnerController->ClearFocus(static_cast<EBTNodeResult::Type>(FocusPriorityToClear));
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
//~ End Task
