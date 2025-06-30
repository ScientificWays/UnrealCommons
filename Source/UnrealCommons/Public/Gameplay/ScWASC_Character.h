// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWASC_Base.h"

#include "ScWASC_Character.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] ASC Character"))
class UNREALCOMMONS_API UScWASC_Character : public UScWASC_Base
{
	GENERATED_BODY()

public:

	UScWASC_Character();
	
//~ Begin Statics
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UScWASC_Character* TryGetCharacterScWASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface = true);
//~ End Statics
};
