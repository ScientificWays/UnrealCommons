// Scientific Ways

#pragma once

#include "UnrealCommons.h"

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

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	FTransform SkeletalMeshRelativeTransform;

	UPROPERTY(Category = "Mesh", EditDefaultsOnly, BlueprintReadOnly)
	bool bRagdollOnDeath;
//~ End Mesh

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "!bRagdollOnDeath"))
	TSubclassOf<UAnimInstance> DiedAnimInstanceClass;
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
	TObjectPtr<class UScWWeaponData_Base> DefaultWeaponData;
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
