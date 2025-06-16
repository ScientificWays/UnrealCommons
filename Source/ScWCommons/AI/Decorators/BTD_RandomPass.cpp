// G.o.o.P Studios

#include "AI/Decorators/BTD_RandomPass.h"

UBTD_RandomPass::UBTD_RandomPass()
{
	NodeName = TEXT("Random pass");
}

//~ Begin Decorator
FString UBTD_RandomPass::GetStaticDescription() const // UBTNode
{
	return FString::Printf(TEXT("%s\nPass with %i%% probability."), *Super::GetStaticDescription(), (int32)(PassProbability * 100.0f));
}

bool UBTD_RandomPass::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const
{
	//if (PassProbability == 0.0f) return false;
	//if (PassProbability == 1.0f) return true;
	return FMath::RandRange(0.0f, 1.0f) < PassProbability;
}
//~ End Decorator
