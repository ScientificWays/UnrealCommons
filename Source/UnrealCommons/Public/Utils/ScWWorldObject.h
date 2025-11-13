// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWWorldObject.generated.h"

/**
* 
*/
UCLASS(Blueprintable, meta = (DisplayName = "[ScW] World Object"))
class UNREALCOMMONS_API UScWWorldObject : public UObject
{
	GENERATED_BODY()

public:	

	UScWWorldObject();
	
//~ Begin World
public:

	//UFUNCTION(Category = "World", BlueprintCallable, meta = (WorldContext = "InWCO"))
	//void UpdateWorldContext(UObject* InWCO);

protected:
	virtual UWorld* GetWorld() const override; // UObject
	//FWeakObjectPtr CurrentWCO;
//~ End World
};
