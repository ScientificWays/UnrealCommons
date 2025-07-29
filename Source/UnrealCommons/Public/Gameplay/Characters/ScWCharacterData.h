// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Animations/ScWTypes_Animations.h"

#include "Gameplay/ScWTypes_Gameplay.h"

#include "ScWCharacterData.generated.h"

/**
 *
 */
UCLASS(Const, Abstract, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Character Data"))
class UNREALCOMMONS_API UScWCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWCharacterData();
	
//~ Begin Initialize
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "InitializeCharacterComponents"))
	void BP_InitializeCharacterComponents(class AScWCharacter* InCharacter) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "InitializeCharacterController"))
	void BP_InitializeCharacterController(class AScWCharacter* InCharacter) const;
//~ End Initialize

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
//~ End UI

//~ Begin Class
public:

	UPROPERTY(Category = "Class", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AScWCharacter> CharacterClass;
//~ End Class

//~ Begin Team
public:

	UFUNCTION(Category = "Team", BlueprintCallable, meta = (WorldContext = "InWCO"))
	FGenericTeamId GetDefaultTeamId(const UObject* InWCO) const;

	UPROPERTY(Category = "Team", EditDefaultsOnly, BlueprintReadOnly)
	FName DefaultTeamName;
//~ End Team
	
//~ Begin Mesh
public:

	UPROPERTY(Category = "Mesh | FirstPerson", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> FP_SkeletalMesh;

	UPROPERTY(Category = "Mesh | FirstPerson", EditDefaultsOnly, BlueprintReadOnly)
	FTransform FP_SkeletalMeshRelativeTransform;

	UPROPERTY(Category = "Mesh | ThirdPerson", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> TP_SkeletalMesh;

	UPROPERTY(Category = "Mesh | ThirdPerson", EditDefaultsOnly, BlueprintReadOnly)
	FTransform TP_SkeletalMeshRelativeTransform;

	UPROPERTY(Category = "Mesh | ThirdPerson", EditDefaultsOnly, BlueprintReadOnly)
	bool bRagdollOnDeath;
//~ End Mesh

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations | FirstPerson", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWAnimInstance_FirstPerson> FP_AnimInstanceClass;

	UPROPERTY(Category = "Animations | FirstPerson", EditAnywhere, BlueprintReadOnly)
	FScWFirstPersonStaticAnimationData FP_DefaultStaticAnimationData;

	UPROPERTY(Category = "Animations | ThirdPerson", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWAnimInstance_ThirdPerson> TP_AnimInstanceClass;

	UPROPERTY(Category = "Animations | ThirdPerson", EditAnywhere, BlueprintReadOnly)
	FScWThirdPersonStaticAnimationData TP_DefaultStaticAnimationData;

	UPROPERTY(Category = "Animations | ThirdPerson", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "!bRagdollOnDeath"))
	TSubclassOf<UAnimInstance> TP_DiedAnimInstanceClass;
//~ End Animations
	
//~ Begin Physics
public:

	UPROPERTY(Category = "Physics", EditDefaultsOnly, BlueprintReadOnly)
	FVector2D CapsuleRadiusHeight;
//~ End Physics
	
//~ Begin Gameplay
public:

	UPROPERTY(Category = "Gameplay", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> SpawnEffectClass;

	UPROPERTY(Category = "Gameplay", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWGameplayGiveAbilityData> DefaultAbilitiesGiveData;

	UPROPERTY(Category = "Gameplay", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UScWHandheldData> DefaultHandheldData;
//~ End Gameplay
	
//~ Begin FX
public:

	UFUNCTION(Category = "FX", BlueprintNativeEvent, BlueprintCallable, BlueprintCosmetic, meta = (WorldContext = "InWCO", DisplayName = "Spawn Damage Impact Particles from GameplayCue", KeyWords = "NiagaraSystem"))
	UNiagaraComponent* BP_SpawnDamageImpactParticlesFromGameplayCue(const UObject* InWCO, const FGameplayCueParameters& InParams) const;

	UPROPERTY(Category = "FX", EditDefaultsOnly, BlueprintReadOnly, meta = (KeyWords = "NiagaraSystem"))
	TObjectPtr<UNiagaraSystem> DamageImpactParticles;

	UPROPERTY(Category = "FX", EditDefaultsOnly, BlueprintReadOnly, meta = (KeyWords = "NiagaraSystem"))
	FLinearColor DamageImpactParticlesColor;
//~ End FX
	
//~ Begin AI
public:

	UPROPERTY(Category = "Class", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AScWAIController> AIControllerClass;

	UPROPERTY(Category = "AI", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;

	UPROPERTY(Category = "AI", EditDefaultsOnly, BlueprintReadOnly)
	float SightRadius;

	UPROPERTY(Category = "AI", EditDefaultsOnly, BlueprintReadOnly)
	float LoseSightRadiusOffset;
//~ End AI
};
