// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWBTD_RandomPass.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMMONS_API UScWBTD_RandomPass : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UScWBTD_RandomPass();
	
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
