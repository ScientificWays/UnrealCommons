// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAIPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "[ScW] AI Patrol Point"))
class UNREALCOMMONS_API AScWAIPatrolPoint : public AActor
{
	GENERATED_BODY()

public:

	AScWAIPatrolPoint();
	
//~ Begin Location
public:

	UFUNCTION(Category = "Location", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetMoveToLocation"))
	FVector BP_GetMoveToLocation() const;
//~ End Location
};
