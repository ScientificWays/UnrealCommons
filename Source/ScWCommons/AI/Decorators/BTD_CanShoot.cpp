// G.o.o.P Studios

#include "AI/Decorators/BTD_CanShoot.h"

#include "AI/ATAAIController_Base.h"

UBTD_CanShoot::UBTD_CanShoot()
{
	NodeName = TEXT("Can shoot");


}

bool UBTD_CanShoot::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const
{
	if (AATAAIController_Base* OwnerController = Cast<AATAAIController_Base>(InOwnerTree.GetAIOwner()))
	{
		if (UBlackboardComponent* OwnerBlackboardComponent = InOwnerTree.GetBlackboardComponent())
		{
			if (APawn* OwnerPawn = OwnerController->GetPawn())
			{
				return /*OwnerController->CanShoot()*/true;
			}
		}
	}
	return false;
}
