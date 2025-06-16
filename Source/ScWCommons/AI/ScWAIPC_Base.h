// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAAIPC_Base.generated.h"

USTRUCT()
struct FPerceptionTargetServiceData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class UBTS_GetPerceptionTarget> Node;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> OwnerTree;

	uint8* NodeMemory;

	FORCEINLINE bool operator==(const FPerceptionTargetServiceData& InOther) const { return NodeMemory == InOther.NodeMemory; }
	FORCEINLINE bool operator!=(const FPerceptionTargetServiceData& InOther) const { return !(*this == InOther); }
};

/**
 *
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UATAAIPC_Base : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:

	UATAAIPC_Base();
	
//~ Begin Initialize
protected:
	virtual void BeginPlay() override; // UActorComponent
//~ End Initialize

//~ Begin Services
public:
	void RegisterGetPerceptionTargetService(class UBTS_GetPerceptionTarget& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory);
	void UnRegisterGetPerceptionTargetService(uint8* InNodeMemory);
protected:
	
	UPROPERTY()
	TArray<FPerceptionTargetServiceData> PerceptionTargetServiceDataArray;
//~ End Services

//~ Begin Update
protected:

	UFUNCTION()
	void OnPerceptionUpdatedCallback(const TArray<AActor*>& InUpdatedActors);
//~ End Update
};
