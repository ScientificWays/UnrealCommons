// Scientific Ways

#include "AI/Tasks/ScWBTT_ClearBlackboardKey.h"

UScWBTT_ClearBlackboardKey::UScWBTT_ClearBlackboardKey()
{
	NodeName = TEXT("Clear Blackboard Key");
}

//~ Begin Task
FString UScWBTT_ClearBlackboardKey::GetStaticDescription() const // UBTNode
{
	FString KeyDescription("invalid");

	if (BlackboardKey.IsSet())
	{
		KeyDescription = BlackboardKey.SelectedKeyName.ToString();
	}
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDescription);
}

EBTNodeResult::Type UScWBTT_ClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) // UBTTaskNode
{
	if (UBlackboardComponent* BlackboardComponent = InOwnerTree.GetBlackboardComponent())
	{
		BlackboardComponent->ClearValue(BlackboardKey.GetSelectedKeyID());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
//~ End Task
