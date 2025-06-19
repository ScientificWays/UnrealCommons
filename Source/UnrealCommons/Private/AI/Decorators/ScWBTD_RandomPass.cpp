// Scientific Ways

#include "AI/Decorators/ScWBTD_RandomPass.h"

UScWBTD_RandomPass::UScWBTD_RandomPass()
{
	NodeName = TEXT("Random Pass");
}

//~ Begin Decorator
FString UScWBTD_RandomPass::GetStaticDescription() const // UBTNode
{
	return FString::Printf(TEXT("%s\nPass with %i%% probability."), *Super::GetStaticDescription(), (int32)(PassProbability * 100.0f));
}

bool UScWBTD_RandomPass::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const
{
	//if (PassProbability == 0.0f) return false;
	//if (PassProbability == 1.0f) return true;
	return FMath::RandRange(0.0f, 1.0f) < PassProbability;
}
//~ End Decorator
