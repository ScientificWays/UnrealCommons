// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAIFunctionLibrary.generated.h"

/**
* 
*/
UCLASS(meta = (DisplayName = "[ScW] AI Function Library"))
class UNREALCOMMONS_API UScWAIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

//~ Begin Blackboard
public:

	UFUNCTION(Category = "Blackboard", BlueprintCallable)
	static float GetDistanceBetweenTwoBlackboardKeys(const UBlackboardComponent* InBlackboard, const FBlackboardKeySelector& InA, const FBlackboardKeySelector& InB, const bool bInSquared = true, const bool bInHorizontal = false);
//~ End Blackboard

//~ Begin Perception
public:

	UFUNCTION(Category = "Perception", BlueprintCallable)
	static int32 GetActorObserversNumFromActorArray(const AActor* InActor, const TArray<AActor*>& InActorArray);
//~ End Perception

//~ Begin Navigation
public:

	UFUNCTION(Category = "Navigation", BlueprintCallable)
	static int32 GetNearestPatrolPointIndex(const FVector& InReferenceLocation, const TArray<class AScWAIPatrolPoint*>& InPatrolPoints, const bool bInUseMoveToLocations = true);

	UFUNCTION(Category = "Navigation", BlueprintCallable)
	static class AScWAIPatrolPoint* GetNearestPatrolPoint(const FVector& InReferenceLocation, const TArray<class AScWAIPatrolPoint*>& InPatrolPoints, const bool bInUseMoveToLocations = true);

	UFUNCTION(Category = "Navigation", BlueprintCallable, meta = (KeyWords = "GetMoveDirection"))
	static FVector GetAIMoveDirection(AAIController* InController, FVector InFallbackDirection = FVector(1.0f, 0.0f, 0.0f), const bool bInChecked = false);
//~ End Navigation

//~ Begin Brain
public:

	UFUNCTION(Category = "Brain", BlueprintCallable)
	static UBrainComponent* TryGetActorBrainComponent(const AActor* InActor);
//~ End Brain
};
