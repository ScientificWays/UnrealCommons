// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWPFC_Base.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "[ScW] AI PFC Base"))
class UNREALCOMMONS_API UScWPFC_Base : public UCrowdFollowingComponent
{
	GENERATED_BODY()

public:

	UScWPFC_Base();
	
//~ Begin Initialize
protected:
	virtual void BeginPlay() override; // UActorComponent
	virtual void EndPlay(const EEndPlayReason::Type InReason) override; // UActorComponent
//~ End Initialize
};
