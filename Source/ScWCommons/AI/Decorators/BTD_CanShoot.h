// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "AI/Decorators/BTD_Tickable.h"

#include "BTD_CanShoot.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTD_CanShoot : public UBTD_Tickable
{
	GENERATED_BODY()

public:

	UBTD_CanShoot();

//~ Begin Decorator
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
//~ End Decorator
};
