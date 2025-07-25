// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWTypes_Animations.h"

#include "ScWAnimInstance_Base.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNREALCOMMONS_API UScWAnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UScWAnimInstance_Base();
	
//~ Begin Editor
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent) override; // UObject
#endif // WITH_EDITOR	
//~ End Editor
	
//~ Begin Initialize
protected:
	virtual void NativeInitializeAnimation() override; // UAnimInstance
//~ End Initialize

//~ Begin Owner
protected:

	UPROPERTY(Category = "Owner", BlueprintReadOnly)
	TObjectPtr<class AScWCharacter> OwnerCharacter;
//~ End Owner
	
//~ Begin Static
public:
	virtual void ResetToDefaultStaticAnimationData() {}
	void SetUseHolsterStaticAnimationData(const bool bInUseHolster, const float InBlendTime);
	void SetUseAlternativeBasePose(const bool bInUseAlternative, const float InBlendTime);
	void SetEnableIdleAnimation(const bool bInEnable);
	
	UFUNCTION(Category = "Static", BlueprintImplementableEvent, meta = (DisplayName = "OnCurrentStaticAnimationDataChanged"))
	void BP_OnCurrentStaticAnimationDataChanged();
	
	UFUNCTION(Category = "Static", BlueprintImplementableEvent, meta = (DisplayName = "OnDefaultStaticAnimationDataChanged"))
	void BP_OnDefaultStaticAnimationDataChanged();
	
	UFUNCTION(Category = "Static", BlueprintImplementableEvent, meta = (DisplayName = "OnEnableHolsterStaticAnimationDataChanged"))
	void BP_OnUseHolsterStaticAnimationDataChanged();
	
	UFUNCTION(Category = "Static", BlueprintImplementableEvent, meta = (DisplayName = "OnUseAlternativeBasePoseChanged"))
	void BP_OnUseAlternativeBasePoseChanged();
	
	UFUNCTION(Category = "Static", BlueprintImplementableEvent, meta = (DisplayName = "OnEnableIdleAnimationChanged"))
	void BP_OnEnableIdleAnimationChanged();
	
protected:

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditAnywhere)
	bool bUseHolsterStaticAnimationData;
	
	UPROPERTY(Category = "Static", BlueprintReadOnly, EditAnywhere)
	float CurrentHolsterStaticAnimationDataBlendTime;

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditAnywhere)
	bool bUseAlternativeBasePose;

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditAnywhere)
	float CurrentAlternativeBasePoseBlendTime;

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditAnywhere)
	bool bEnableIdleAnimation;
//~ End Static
};
