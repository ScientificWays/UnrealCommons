// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/ScWTypes_AI.h"

#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "ScWBTS_LookAt.generated.h"

/**
 *	Set focal point of Owner from BlackboardKey and optional rotation offset.
 *	If key is not set, distance of 1000 cm is used.
 */
UCLASS()
class UNREALCOMMONS_API UScWBTS_LookAt : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UScWBTS_LookAt();
	
//~ Begin Editor
#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) override; // UObject
#endif // WITH_EDITOR	
//~ End Editor

//~ Begin Service
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual void InitializeFromAsset(UBehaviorTree& InTreeAsset) override; // UBTNode
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTAuxiliaryNode
	virtual void TickNode(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, float InDeltaSeconds) override; // UBTAuxiliaryNode
//~ End Service

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	EATAAIFocusPriority FocusPriority;

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bClearFocusOnCeaseRelevant;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!bFocusOnActor"))
	FRotator Offset;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (EditCondition = "!bFocusOnActor"))
	bool bLookAtPawnViewLocation;

	/* If set, will set focus actor in Controller on become relevant instead of setting focal point every tick */
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bFocusOnActor;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bDrawDebug;
#endif // WITH_EDITORONLY_DATA
//~ End Settings
};
