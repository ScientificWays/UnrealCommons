// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWTypes_Animations.h"

#include "Gameplay/ScWTypes_Gameplay.h"

#include "ScWHandheldData.generated.h"

/**
 *
 */
UCLASS(Const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Handheld Data (Base)"))
class UNREALCOMMONS_API UScWHandheldData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWHandheldData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
//~ End UI

//~ Begin General
public:

	UPROPERTY(Category = "General", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AScWHandheld> HandheldClass;

	UPROPERTY(Category = "General", EditDefaultsOnly, BlueprintReadOnly)
	bool bCanDrop;

	UPROPERTY(Category = "General", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bCanDrop"))
	bool bDropOnDeath;
//~ End General
	
//~ Begin Mesh
public:

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	FName FP_OwnerMeshAttachmentSocketName;

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	FName TP_OwnerMeshAttachmentSocketName;
//~ End Mesh

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations | Handheld", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWAnimInstance_Handheld> HandheldAnimInstanceClass;

	UPROPERTY(Category = "Animations | Handheld", EditAnywhere, BlueprintReadOnly)
	FScWHandheldStaticAnimationData HandheldStaticAnimationData;

	//UPROPERTY(Category = "Animations | Owner FirstPerson", EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<class UScWAnimInstance_FirstPerson> FP_AnimInstanceClass;

	UPROPERTY(Category = "Animations | Owner FirstPerson", EditAnywhere, BlueprintReadOnly)
	FScWFirstPersonStaticAnimationData FP_StaticAnimationData;

	//UPROPERTY(Category = "Animations | Owner ThirdPerson", EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<class UScWAnimInstance_ThirdPerson> TP_AnimInstanceClass;

	UPROPERTY(Category = "Animations | Owner ThirdPerson", EditAnywhere, BlueprintReadOnly)
	FScWThirdPersonStaticAnimationData TP_StaticAnimationData;
//~ End Animations
	
//~ Begin Abilities
public:

	UPROPERTY(Category = "Abilities", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWGameplayGiveAbilityData> HandheldAbilitiesGiveData;
//~ End Abilities
};
