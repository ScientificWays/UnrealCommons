// G.o.o.P Studios

#include "AI/Decorators/BTD_Tickable.h"

UBTD_Tickable::UBTD_Tickable()
{
	NodeName = TEXT("Tickable Base");
	bNotifyTick = true;
	bTickIntervals = true;

	TickRate = -1.0f;
}

//~ Begin Decorator
void UBTD_Tickable::InitializeFromAsset(UBehaviorTree& InTreeAsset) // UBTNode
{
	Super::InitializeFromAsset(InTreeAsset);
	bNotifyTick = TickRate > 0.0f;
}

void UBTD_Tickable::TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) // UBTAuxiliaryNode
{
	Super::TickNode(InOwnerTree, InNodeMemory, InDeltaSeconds);

	ConditionalFlowAbort(InOwnerTree, EBTDecoratorAbortRequest::ConditionResultChanged);
	SetNextTickTime(InNodeMemory, TickRate);
}
//~ End Decorator
