// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAAIFunctionLibrary.generated.h"

/**
* 
*/
UCLASS()
class ATA_DREAMCOMETRUE_API UATAAIFunctionLibrary : public UBlueprintFunctionLibrary
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
};
