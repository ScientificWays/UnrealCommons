// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "AI/Decorators/BTD_Tickable.h"

#include "BTD_CheckOcclusion.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UBTD_CheckOcclusion : public UBTD_Tickable
{
	GENERATED_BODY()
	
public:

	UBTD_CheckOcclusion();

//~ Begin Decorator
protected:
	virtual FString GetStaticDescription() const; // UBTNode
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) const override; // UBTDecorator
//~ End Decorator

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector FromTargetKey;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "FromTargetKey.SelectedKeyName != NAME_None"))
	FVector FromTargetLocationOffset;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bCheckFromPawnViewLocation;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FBlackboardKeySelector ToTargetKey;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "ToTargetKey.SelectedKeyName != NAME_None"))
	FVector ToTargetLocationOffset;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bCheckToPawnViewLocation;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (ClampMin = "-1.0"))
	float MaxCheckDistance;

	UPROPERTY(Category = "Settings", EditAnywhere)
	FRotator Offset;

	UPROPERTY(Category = "Settings", EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(Category = "Settings", EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceType;
//~ End Settings
};
