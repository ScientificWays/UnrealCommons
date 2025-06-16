// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWASC_Base.h"

#include "ScWASC_Character.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOMMONS_API UScWASC_Character : public UScWASC_Base
{
	GENERATED_BODY()

public:

	UScWASC_Character();
	
//~ Begin Statics
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UScWASC_Character* TryGetCharacterAtaASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface = true);
//~ End Statics

//~ Begin Initialize
public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override; // UAbilitySystemComponent
protected:
	virtual void OnRegister() override; // UActorComponent
//~ End Initialize

//~ Begin Components
public:

	UPROPERTY(Category = "Components", BlueprintReadOnly)
	TObjectPtr<class UScWCMC_Base> OwnerCMC;
//~ End Components

//~ Begin Attributes
public:

protected:
	virtual void OnMaxWalkSpeedChanged(const FOnAttributeChangeData& InData);
	virtual void OnMaxWalkSpeedCrouchedChanged(const FOnAttributeChangeData& InData);
	virtual void OnMaxAccelerationChanged(const FOnAttributeChangeData& InData);
	virtual void OnBrakingDecelerationWalkingChanged(const FOnAttributeChangeData& InData);
	virtual void OnGravityScaleChanged(const FOnAttributeChangeData& InData);
	virtual void OnAirControlChanged(const FOnAttributeChangeData& InData);

	FDelegateHandle MaxWalkSpeedChangedDelegateHandle;
	FDelegateHandle MaxWalkSpeedCrouchedChangedDelegateHandle;
	FDelegateHandle MaxAccelerationChangedDelegateHandle;
	FDelegateHandle BrakingDecelerationWalkingChangedDelegateHandle;
	FDelegateHandle GravityScaleChangedDelegateHandle;
	FDelegateHandle AirControlChangedDelegateHandle;
//~ End Attributes

//~ Begin Tags
protected:
	virtual void OnMovementModeTagChanged(const FGameplayTag InCallbackTag, int32 InNewCount);
	virtual void UpdateCharacterMovementModeFromTags();
//~ End Tags
};
