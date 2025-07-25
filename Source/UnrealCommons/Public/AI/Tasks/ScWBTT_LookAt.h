// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "AI/ScWTypes_AI.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "ScWBTT_LookAt.generated.h"

/**
 *	Set focal point of Owner from BlackboardKey and optional rotation offset.
 *	If key is not set, distance of 1000 cm is used.
 */
UCLASS(meta = (DisplayName = "[ScW] BTT_LookAt(Focus)"))
class UNREALCOMMONS_API UScWBTT_LookAt : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UScWBTT_LookAt();
	
//~ Begin Task
protected:
	virtual FString GetStaticDescription() const override; // UBTNode
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory) override; // UBTTaskNode
public:
	static EBTNodeResult::Type Common_SetFocalPoint(UBTNode& InNode, UBehaviorTreeComponent& InOwnerTree, uint8* InNodeMemory, const FBlackboardKeySelector& InBlackboardKey, const EScWAIFocusPriority InFocusPriority, const FRotator& InOffset, const bool bInLookAtPawnViewLocation, const bool bInDrawDebug = false);
//~ End Task

//~ Begin Settings
protected:

	UPROPERTY(Category = "Settings", EditAnywhere)
	EScWAIFocusPriority FocusPriority;

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
