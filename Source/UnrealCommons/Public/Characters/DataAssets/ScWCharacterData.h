// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWCharacterData.generated.h"

/**
 *
 */
UCLASS(const, Abstract, Blueprintable, BlueprintType)
class UNREALCOMMONS_API UScWCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWCharacterData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "UpdateCharacterFromDataAsset"))
	void K2_UpdateCharacterFromDataAsset(class AScWCharacter* InCharacter) const;

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
//~ End Mesh

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
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
	TMap<TSubclassOf<UGameplayAbility>, int32> DefaultAbilities;

	UPROPERTY(Category = "Gameplay", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UScWWeaponData_Base> DefaultWeaponData;
//~ End Gameplay
	
//~ Begin AI
public:

	UPROPERTY(Category = "AI", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree = nullptr;
//~ End AI
};
