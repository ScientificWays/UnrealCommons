// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWAnimInstance_Base.h"

#include "ScWAnimInstance_FirstPersonLegs.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API UScWAnimInstance_FirstPersonLegs : public UScWAnimInstance_Base
{
	GENERATED_BODY()
	
public:

	UScWAnimInstance_FirstPersonLegs();

//~ Begin Static
public:

	UFUNCTION(Category = "Static", BlueprintCallable)
	void SetIncapped(const bool InValue);
	
	UFUNCTION(Category = "Static", BlueprintImplementableEvent, meta = (DisplayName = "OnUpdateIncapped"))
	void BP_OnUpdateIncapped();

protected:

	UPROPERTY(Category = "Static", EditAnywhere, BlueprintReadOnly)
	bool bIncapped;
//~ End Static
};
