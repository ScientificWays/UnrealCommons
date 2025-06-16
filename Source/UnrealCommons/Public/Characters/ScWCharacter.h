// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_Delegates.h"

#include "ScWCharacter.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UNREALCOMMONS_API AScWCharacter : public ACharacter, public IGenericTeamAgentInterface, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IGameplayCueInterface
{
	GENERATED_BODY()

public:

	AScWCharacter(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, meta = (WorldContext = "InWCO"))
	static AScWCharacter* SpawnCharacter(const UObject* InWCO, UScWCharacterData* InData, FTransform InTransform);
//~ End Statics

//~ Begin Initialize
public:

	UFUNCTION(Category = "Initialize", BlueprintCallable, meta = (KeyWords = "GetCharacterDataAsset"))
	const class UScWCharacterData* GetDataAsset() const { return DataAsset; }

protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor
	virtual void EndPlay(const EEndPlayReason::Type InReason) override; // AActor

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const class UScWCharacterData> DataAsset;
//~ End Initialize

//~ Begin Components
public:
	virtual UActorComponent* FindComponentByClass(const TSubclassOf<UActorComponent> InComponentClass) const override; // AActor
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetAbilitySystemComponent"))
	class UScWASC_Character* GetCharacterASC() const { return CharacterASC; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterMovementComponent, GetCMC"))
	class UScWCMC_Base* GetBaseCMC() const { return BaseCMC; }

protected:

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWASC_Character> CharacterASC;

	UPROPERTY(Category = "Components", BlueprintReadOnly)
	TObjectPtr<class UScWCMC_Base> BaseCMC;
//~ End Components

//~ Begin Controller
protected:
	virtual void PossessedBy(AController* InController) override; // APawn
//~ End Controller

//~ Begin Team
public:
	virtual FGenericTeamId GetGenericTeamId() const override; // IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& InNewTeamID) override; // IGenericTeamAgentInterface
//~ End Team

//~ Begin GameplayTags
public:
	FORCEINLINE virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutTagContainer) const override; // IGameplayTagAssetInterface
	FORCEINLINE virtual bool HasMatchingGameplayTag(FGameplayTag InTagToCheck) const override; // IGameplayTagAssetInterface
	FORCEINLINE virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& InTagContainer) const override; // IGameplayTagAssetInterface
	FORCEINLINE virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& InTagContainer) const override; // IGameplayTagAssetInterface
//~ End GameplayTags

//~ Begin GameplayCue
public:
	virtual void HandleGameplayCue(UObject* InSelf, FGameplayTag InGameplayCueTag, EGameplayCueEvent::Type InEventType, const FGameplayCueParameters& InParameters) override; // IGameplayCueInterface
//~ End GameplayCue

//~ Begin Weapon
public:

	UFUNCTION(Category = "Weapon", BlueprintCallable, meta = (KeyWords = "GetCurrentWeapon"))
	class AScWWeapon_Base* GetWeapon() const { return Weapon; }

	UFUNCTION(Category = "Weapon", BlueprintCallable)
	class AScWWeapon_Base* GiveWeapon(class UScWWeaponData_Base* InWeaponData);

	UFUNCTION(Category = "Weapon", BlueprintCallable)
	void RemoveWeapon() { GiveWeapon(nullptr); }

	UPROPERTY(Category = "Weapon", BlueprintAssignable)
	FDefaultEventSignature OnWeaponChanged;

protected:

	UPROPERTY(Category = "Weapon", BlueprintReadOnly)
	TObjectPtr<class AScWWeapon_Base> Weapon;

	UPROPERTY(Category = "Weapon", BlueprintReadOnly)
	TArray<FGameplayAbilitySpecHandle> WeaponAbilitiesHandleArray;
//~ End Weapon
};
