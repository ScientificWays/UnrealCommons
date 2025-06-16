// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "BTS_GetDistanceTo.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTS_GetDistanceTo : public UBTService
{
	GENERATED_BODY()

public:

	UBTS_GetDistanceTo();
	
//~ Begin Editor
#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) override; // UObject
#endif // WITH_EDITOR	
//~ End Editor

//~ Begin Service
protected:
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual void TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTAuxiliaryNode
//~ End Service

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector FromTargetKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector ToTargetKey;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bGetSquaredDistance;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bGetHorizontalDistance;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bGetVerticalDistance;
//~ End Settings

//~ Begin Output
	UPROPERTY(Category = "Output", EditAnywhere)
	FBlackboardKeySelector OutDistanceKey;
//~ End Output
};
