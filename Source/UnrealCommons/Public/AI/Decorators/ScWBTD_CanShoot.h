// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/Decorators/ScWBTD_Tickable.h"

#include "ScWBTD_CanShoot.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] BTD_CanShoot"))
class UNREALCOMMONS_API UScWBTD_CanShoot : public UScWBTD_Tickable
{
	GENERATED_BODY()

public:

	UScWBTD_CanShoot();

//~ Begin Decorator
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
//~ End Decorator
};
