// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWASC_InitInterface.h"

#include "ScWCharacterMovement.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "[ScW] Character Movement (Component)"))
class UNREALCOMMONS_API UScWCharacterMovement : public UCharacterMovementComponent, public IScWASC_InitInterface
{
	GENERATED_BODY()
	
public:	
	
	UScWCharacterMovement();
	
//~ Begin Initialize
public:
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) override; // IScWASC_InitInterface
//~ End Initialize

//~ Begin Attributes
public:

protected:
	void OnMaxWalkSpeedAttributeChanged(const FOnAttributeChangeData& InData);
	void OnMaxWalkSpeedCrouchedAttributeChanged(const FOnAttributeChangeData& InData);
	void OnMaxAccelerationAttributeChanged(const FOnAttributeChangeData& InData);
	void OnBrakingDecelerationWalkingAttributeChanged(const FOnAttributeChangeData& InData);
	void OnGravityScaleAttributeChanged(const FOnAttributeChangeData& InData);
	void OnAirControlAttributeChanged(const FOnAttributeChangeData& InData);
	void OnMassAttributeChanged(const FOnAttributeChangeData& InData);

	FDelegateHandle MaxWalkSpeedChangedDelegateHandle;
	FDelegateHandle MaxWalkSpeedCrouchedChangedDelegateHandle;
	FDelegateHandle MaxAccelerationChangedDelegateHandle;
	FDelegateHandle BrakingDecelerationWalkingChangedDelegateHandle;
	FDelegateHandle GravityScaleChangedDelegateHandle;
	FDelegateHandle AirControlChangedDelegateHandle;
	FDelegateHandle MassChangedDelegateHandle;
//~ End Attributes

//~ Begin Tags
protected:
	void OnMovementModeTagNumChanged(const FGameplayTag InCallbackTag, int32 InNewCount);
	void UpdateCharacterMovementModeFromTags();
//~ End Tags

//~ Begin Damage
public:

	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite)
	FName RagdollCollisionProfileName;

protected:

	UFUNCTION()
	void HandleDamageApplied(float InDamage, const struct FReceivedDamageData& InData);

	UFUNCTION()
	void HandleDied();
//~ End Damage
};
