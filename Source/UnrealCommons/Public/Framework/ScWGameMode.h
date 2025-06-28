// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWGameMode.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Game Mode"))
class UNREALCOMMONS_API AScWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AScWGameMode();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static AScWGameMode* TryGetScWGameMode(const UObject* InWCO);
//~ End Statics
};
