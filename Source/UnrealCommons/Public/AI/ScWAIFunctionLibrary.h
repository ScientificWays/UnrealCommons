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
	static int32 GetNearestPatrolPointIndex(const FVector& InReferenceLocation, const TArray<class AScWAIPatrolPoint*>& InPatrolPoints, const bool bInCheckMoveToLocations = true);

	UFUNCTION(Category = "Navigation", BlueprintCallable)
	static class AScWAIPatrolPoint* GetNearestPatrolPoint(const FVector& InReferenceLocation, const TArray<class AScWAIPatrolPoint*>& InPatrolPoints, const bool bInCheckMoveToLocations = true);
//~ End Navigation
};
