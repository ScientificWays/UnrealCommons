// Scientific Ways

#include "AI/Decorators/ScWBTD_Tickable.h"

UScWBTD_Tickable::UScWBTD_Tickable()
{
	NodeName = TEXT("Tickable Base");
	bNotifyTick = true;
	bTickIntervals = true;

	TickRate = -1.0f;
}

//~ Begin Decorator
void UScWBTD_Tickable::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);
	bNotifyTick = TickRate > 0.0f;
}

void UScWBTD_Tickable::TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTAuxiliaryNode
{
	Super::TickNode(InOwnerTree, InNodeMemory, InDeltaSeconds);

	ConditionalFlowAbort(InOwnerTree, EBTDecoratorAbortRequest::ConditionResultChanged);
	SetNextTickTime(InNodeMemory, TickRate);
}
//~ End Decorator
