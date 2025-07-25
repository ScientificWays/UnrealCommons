// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWAnimInstance_Base.h"

#include "ScWAnimInstance_FirstPerson.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API UScWAnimInstance_FirstPerson : public UScWAnimInstance_Base
{
	GENERATED_BODY()
	
public:

	UScWAnimInstance_FirstPerson();
	
//~ Begin Static
public:
	virtual void ResetToDefaultStaticAnimationData() override { SetCurrentStaticAnimationData(DefaultStaticAnimationData); }
	void SetCurrentStaticAnimationData(const FScWFirstPersonStaticAnimationData& InStaticAnimationData);
	void SetDefaultStaticAnimationData(const FScWFirstPersonStaticAnimationData& InStaticAnimationData);
protected:

	UPROPERTY(Category = "Static", EditAnywhere, BlueprintReadOnly)
	FScWFirstPersonStaticAnimationData CurrentStaticAnimationData;

	UPROPERTY(Category = "Static", EditAnywhere, BlueprintReadOnly)
	FScWFirstPersonStaticAnimationData DefaultStaticAnimationData;
//~ End Static
};
