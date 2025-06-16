// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "BTD_RandomPass.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTD_RandomPass : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UBTD_RandomPass();
	
//~ Begin Decorator
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
//~ End Decorator

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PassProbability = 0.5f;
//~ End Settings
};
