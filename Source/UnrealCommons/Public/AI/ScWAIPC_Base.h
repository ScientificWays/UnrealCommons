// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAIPC_Base.generated.h"

USTRUCT()
struct FPerceptionTargetServiceData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class UScWBTS_GetPerceptionTarget> Node;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> OwnerTree;

	uint8* NodeMemory;

	FORCEINLINE bool operator==(const FPerceptionTargetServiceData& InOther) const { return NodeMemory == InOther.NodeMemory; }
	FORCEINLINE bool operator!=(const FPerceptionTargetServiceData& InOther) const { return !(*this == InOther); }
};

/**
 *
 */
UCLASS(meta = (DisplayName = "ScW AI PC Base"))
class UNREALCOMMONS_API UScWAIPC_Base : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:

	UScWAIPC_Base();
	
//~ Begin Initialize
protected:
	virtual void BeginPlay() override; // UActorComponent
//~ End Initialize

//~ Begin Services
public:
	void RegisterGetPerceptionTargetService(class UScWBTS_GetPerceptionTarget& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory);
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
