// G.o.o.P Studios

#include "AI/Tasks/BTT_LookAt_Clear.h"

#include "AI/Types_AI.h"
#include "AI/ATAAIController_Base.h"

UBTT_LookAt_Clear::UBTT_LookAt_Clear()
{
	NodeName = "Clear look at";

	FocusPriorityToClear = EATAAIFocusPriority::Gameplay3;
	bClearAllFocuses = false;
}

//~ Begin Task
FString UBTT_LookAt_Clear::GetStaticDescription() const // UBTNode
{
	static const UEnum* FocusPriorityEnum = StaticEnum<EATAAIFocusPriority>();
	FString FocusPriorityName = FocusPriorityEnum->GetNameStringByIndex(static_cast<int32>(FocusPriorityToClear));
	return bClearAllFocuses ? TEXT("Clear all focuses") : FString::Printf(TEXT("Clear %s focus"), *FocusPriorityName);
}

EBTNodeResult::Type UBTT_LookAt_Clear::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	if (AATAAIController_Base* OwnerController = Cast<AATAAIController_Base>(InOwnerTree.GetAIOwner()))
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
