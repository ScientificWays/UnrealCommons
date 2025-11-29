// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Handhelds/ScWHandheldData.h"
#include "Gameplay/Handhelds/ScWTypes_Handhelds.h"

#include "ScWHandheldData_Melee.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Handheld Data (Melee)"))
class UNREALCOMMONS_API UScWHandheldData_Melee : public UScWHandheldData
{
	GENERATED_BODY()

public:	

	UScWHandheldData_Melee();
	
//~ Begin Physics
public:

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly)
	bool bIsUsingCollisionComponent;

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingCollisionComponent"))
	FVector2D CapsuleRadiusHeight;

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingCollisionComponent"))
	FTransform CapsuleRelativeTransform;
//~ End Physics
	
//~ Begin Movement
public:

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	bool bPushPlayerDuringSwing;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingCollisionComponent"))
	FVector SwingPushVector;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsUsingCollisionComponent"))
	float SwingPushDuration;
//~ End Movement
	
//~ Begin Swing
public:

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float SwingBaseDamage;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWDamageType> SwingBaseDamageTypeClass;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> SwingOwnerEffect;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadOnly)
	bool bSwingOwnerEffectOnlyDuringSwing;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float PostSwingComboTimeWindow;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float SwingAIMaxRange;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	FName SwingAIMaxRange_BlackboardKeyName;
//~ End Swing

//~ Begin Swing Variants
public:

	UPROPERTY(Category = "Swing Variants", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWMeleeSwingVariantData> SwingVariants;

	UPROPERTY(Category = "Swing Variants", EditDefaultsOnly, BlueprintReadOnly)
	float SwingVariantBaseDuration;
//~ End Swing Variants

//~ Begin Swing FX
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "InitializeSwingParticles"))
	UNiagaraComponent* BP_InitializeSwingParticles(class AScWHandheld_Melee* InMeleeHandheld) const;

	UPROPERTY(Category = "Swing FX", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> DefaultSwingParticles;

	UPROPERTY(Category = "Swing FX", EditDefaultsOnly, BlueprintReadOnly)
	FName SwingParticlesAttachmentSocketName;

	UPROPERTY(Category = "Swing FX", EditDefaultsOnly, BlueprintReadOnly)
	FTransform SwingParticlesRelativeTransform;
//~ End Swing FX
};
