// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWAnimInstance_Base.h"

#include "ScWAnimInstance_Handheld.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API UScWAnimInstance_Handheld : public UScWAnimInstance_Base
{
	GENERATED_BODY()
	
public:

	UScWAnimInstance_Handheld();
	
//~ Begin Static
public:
	virtual void ResetToDefaultStaticAnimationData() override { SetCurrentStaticAnimationData(DefaultStaticAnimationData); }
	void SetCurrentStaticAnimationData(const FScWHandheldStaticAnimationData& InStaticAnimationData);
protected:

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditDefaultsOnly)
	FScWHandheldStaticAnimationData CurrentStaticAnimationData;

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditDefaultsOnly)
	FScWHandheldStaticAnimationData DefaultStaticAnimationData;
//~ End Static
};
