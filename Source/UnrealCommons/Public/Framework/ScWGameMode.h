// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API AScWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AScWGameMode();

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static AScWGameMode* TryGetScWGameMode(const UObject* InWCO);

};
