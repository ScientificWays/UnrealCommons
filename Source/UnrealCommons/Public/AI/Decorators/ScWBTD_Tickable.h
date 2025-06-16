// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWBTD_Tickable.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API UScWBTD_Tickable : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UScWBTD_Tickable();

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
