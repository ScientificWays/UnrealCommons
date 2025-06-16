// G.o.o.P Studios

#include "AI/Tasks/BTT_ClearBlackboardKey.h"

UBTT_ClearBlackboardKey::UBTT_ClearBlackboardKey()
{
	NodeName = TEXT("Clear Blackboard Key");
}

//~ Begin Task
FString UBTT_ClearBlackboardKey::GetStaticDescription() const // UBTNode
{
	FString KeyDescription("invalid");

	if (BlackboardKey.IsSet())
	{
		KeyDescription = BlackboardKey.SelectedKeyName.ToString();
	}
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDescription);
}

EBTNodeResult::Type UBTT_ClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	if (UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent())
	{
		BlackboardComponent->ClearValue(BlackboardKey.GetSelectedKeyID());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
//~ End Task
