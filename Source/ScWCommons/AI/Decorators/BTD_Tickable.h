// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "BTD_Tickable.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ATA_DREAMCOMETRUE_API UBTD_Tickable : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UBTD_Tickable();

//~ Begin Decorator
protected:
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual void TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTAuxiliaryNode
//~ End Decorator

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "-1.0"))
	float TickRate;
//~ End Settings
};
