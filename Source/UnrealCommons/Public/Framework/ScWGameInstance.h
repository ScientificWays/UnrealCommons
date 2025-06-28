// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Engine/GameInstance.h"

#include "ScWGameInstance.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Game Instance"))
class UNREALCOMMONS_API UScWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UScWGameInstance();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure)
	static UScWGameInstance* TryGetScWGameInstance(const UObject* InWCO);
//~ End Statics
	
};
