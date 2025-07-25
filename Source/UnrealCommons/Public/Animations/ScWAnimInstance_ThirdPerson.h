// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWAnimInstance_Base.h"

#include "ScWAnimInstance_ThirdPerson.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API UScWAnimInstance_ThirdPerson : public UScWAnimInstance_Base
{
	GENERATED_BODY()
	
public:

	UScWAnimInstance_ThirdPerson();
	
//~ Begin Static
public:
	virtual void ResetToDefaultStaticAnimationData() override { SetCurrentStaticAnimationData(DefaultStaticAnimationData); }
	void SetCurrentStaticAnimationData(const FScWThirdPersonStaticAnimationData& InStaticAnimationData);
	void SetDefaultStaticAnimationData(const FScWThirdPersonStaticAnimationData& InStaticAnimationData);
	//void SetHideUpperBody(const bool bHide);
	
	//UFUNCTION(Category = "Static", BlueprintImplementableEvent, meta = (DisplayName = "OnUpdateHideUpperBody"))
	//void BP_OnUpdateHideUpperBody();
	
protected:

	UPROPERTY(Category = "Static", EditAnywhere, BlueprintReadOnly)
	FScWThirdPersonStaticAnimationData CurrentStaticAnimationData;

	UPROPERTY(Category = "Static", EditAnywhere, BlueprintReadOnly)
	FScWThirdPersonStaticAnimationData DefaultStaticAnimationData;
	
	//UPROPERTY(Category = "Static", EditAnywhere, BlueprintReadOnly)
	//bool bHideUpperBody;
//~ End Static
};
