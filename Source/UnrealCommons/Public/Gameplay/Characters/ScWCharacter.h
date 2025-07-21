// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "ScWCharacter.generated.h"

//UDELEGATE()
//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FScWCharacterInputTriggeredEventSignature, AScWCharacter, OnInputTriggered);

/**
 *
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "[ScW] Character"))
class UNREALCOMMONS_API AScWCharacter : public ACharacter, public IGenericTeamAgentInterface, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IGameplayCueInterface
{
	GENERATED_BODY()

public:

	AScWCharacter(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, meta = (WorldContext = "InWCO"))
	static AScWCharacter* SpawnCharacter(const UObject* InWCO, class UScWCharacterData* InData, FTransform InTransform);
//~ End Statics

//~ Begin Initialize
public:

	UFUNCTION(Category = "Initialize", BlueprintCallable)
	bool IsPlayerCharacter() const { return bIsPlayerCharacter; }

	UFUNCTION(Category = "Initialize", BlueprintCallable, meta = (KeyWords = "GetCharacterDataAsset"))
	const class UScWCharacterData* GetDataAsset() const { return DataAsset; }

protected:
	virtual void PostInitializeComponents() override; // AActor
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor
	virtual void EndPlay(const EEndPlayReason::Type InReason) override; // AActor

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	bool bIsPlayerCharacter;

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const class UScWCharacterData> DataAsset;
//~ End Initialize

//~ Begin Components
public:
	virtual UActorComponent* FindComponentByClass(const TSubclassOf<UActorComponent> InComponentClass) const override; // AActor
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetAbilitySystemComponent"))
	class UScWASC_Character* GetCharacterASC() const { return CharacterASC; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterMesh, GetMesh", DisplayName = "Get ScW Character Mesh"))
	class UScWCharacterMesh* GetScWCharacterMesh() const { return ScWCharacterMesh; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterMovementComponent, GetCMC", DisplayName = "Get ScW Character Movement"))
	class UScWCharacterMovement* GetScWCharacterMovement() const { return ScWCharacterMovement; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterCapsuleComponent, GetCapsule", DisplayName = "Get ScW Character Capsule"))
	class UScWCharacterCapsule* GetScWCharacterCapsule() const { return ScWCharacterCapsule; }

protected:

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UScWASC_Character> CharacterASC;

	UPROPERTY(Category = "Components", BlueprintReadOnly, meta = (DisplayName = "ScW Character Mesh"))
	TObjectPtr<class UScWCharacterMesh> ScWCharacterMesh;

	UPROPERTY(Category = "Components", BlueprintReadOnly, meta = (DisplayName = "ScW Character Movement"))
	TObjectPtr<class UScWCharacterMovement> ScWCharacterMovement;

	UPROPERTY(Category = "Components", BlueprintReadOnly, meta = (DisplayName = "ScW Character Capsule"))
	TObjectPtr<class UScWCharacterCapsule> ScWCharacterCapsule;
//~ End Components

//~ Begin Controller
public:
	virtual FVector GetPawnViewLocation() const override; // APawn
protected:
	virtual void SpawnDefaultController() override; // APawn
	virtual void PossessedBy(AController* InController) override; // APawn
	virtual void UnPossessed() override; // APawn
	virtual void NotifyControllerChanged() override; // APawn
//~ End Controller
	
//~ Begin Attributes
protected:

	UFUNCTION()
	void OnDied();
//~ End Attributes

//~ Begin Input
public:

	UPROPERTY(Category = "Input", BlueprintAssignable)
	FDefaultEventSignature OnForwardInputTriggeredDelegate;
	
	UPROPERTY(Category = "Input", BlueprintAssignable)
	FDefaultEventSignature OnBackwardInputTriggeredDelegate;
	
	UPROPERTY(Category = "Input", BlueprintAssignable)
	FDefaultEventSignature OnRightInputTriggeredDelegate;
	
	UPROPERTY(Category = "Input", BlueprintAssignable)
	FDefaultEventSignature OnLeftInputTriggeredDelegate;
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override; // APawn
	void InputHorizontalMovement(const FInputActionInstance& InActionInstance);
	void InputHorizontalMovement_Absolute(const FInputActionInstance& InActionInstance);
	void InputAbilityConfirmPressed();
	void InputAbilityCancelPressed();
	void InputUsePrimaryPressed();
	void InputUsePrimaryReleased();
	void InputUseSecondaryPressed();
	void InputUseSecondaryReleased();
	void InputJumpPressed();
	void InputJumpReleased();
	void InputCrouchPressed();
	void InputCrouchReleased();
	void InputInteractPressed();
	void InputInteractReleased();
	void InputReloadPressed();
	void InputReloadReleased();
	void InputSprintPressed();
	void InputSprintReleased();
	void InputWalkPressed();
	void InputWalkReleased();
	void InputDropPressed();
	void InputDropReleased();
	void InputFlashlightPressed();
	void InputFlashlightReleased();
	void InputWeaponSwitchScrollPressed();
	void InputWeaponSwitchScrollReleased();
	void InputWeaponSwitchDirectPressed();
	void InputWeaponSwitchDirectReleased();

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	int32 DefaultInputMappingContextPriority;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	FModifyContextOptions DefaultInputMappingContextOptions;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> HorizontalMovementAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	bool bHorizontalMovementAbsolute;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> UsePrimaryAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> UseSecondaryAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> AbilityConfirmAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> AbilityCancelAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> DropAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> FlashlightAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> WeaponSwitchScrollAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> WeaponSwitchDirectAction;
//~ End Input
	
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
	class AScWWeapon_Base* GiveWeapon(class UScWWeaponData_Base* InWeaponData, const bool bInDropPrevious = true);

	UFUNCTION(Category = "Weapon", BlueprintCallable)
	void DropWeapon() { GiveWeapon(nullptr, true); }

	UFUNCTION(Category = "Weapon", BlueprintCallable)
	void RemoveWeapon() { GiveWeapon(nullptr, false); }

	UPROPERTY(Category = "Weapon", BlueprintAssignable)
	FDefaultEventSignature OnWeaponChanged;

protected:

	UPROPERTY(Category = "Weapon", BlueprintReadOnly)
	TObjectPtr<class AScWWeapon_Base> Weapon;

	UPROPERTY(Category = "Weapon", BlueprintReadOnly)
	TArray<FGameplayAbilitySpecHandle> WeaponAbilitiesHandleArray;
//~ End Weapon
};
