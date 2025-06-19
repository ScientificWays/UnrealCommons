// Scientific Ways

#include "AI/Decorators/ScWBTD_CanShoot.h"

#include "AI/ScWAIController.h"

UScWBTD_CanShoot::UScWBTD_CanShoot()
{
	NodeName = TEXT("Can Shoot");


}

bool UScWBTD_CanShoot::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const
{
	if (AScWAIController* OwnerController = Cast<AScWAIController>(InOwnerTree.GetAIOwner()))
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
