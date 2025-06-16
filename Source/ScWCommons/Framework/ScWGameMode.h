// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "ATAGameMode_Base.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ATA_DREAMCOMETRUE_API AATAGameMode_Base : public AGameModeBase
{
	GENERATED_BODY()

public:

	AATAGameMode_Base();

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static AATAGameMode_Base* TryGetAtaGameMode(const UObject* InWCO);

};
