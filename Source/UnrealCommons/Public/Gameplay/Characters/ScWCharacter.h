// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "GameAnalytics.h"
#include "GameAnalyticsModule.h"

#include "ScWCharacter.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScWHandheldSignature, class AScWHandheld*, InPrevHandheld, class AScWHandheld*, InNewHandheld);

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

	UFUNCTION(Category = "Initialize", BlueprintImplementableEvent, meta = (DisplayName = "Post Death Custom Behavior"))
	void BP_PostDeathCustomBehavior();

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	bool bIsPlayerCharacter;

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	bool bStartInFirstPersonView;

	UPROPERTY(Category = "Initialize", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const class UScWCharacterData> DataAsset;
//~ End Initialize

//~ Begin Components
public:
	virtual UActorComponent* FindComponentByClass(const TSubclassOf<UActorComponent> InComponentClass) const override; // AActor
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // IAbilitySystemInterface

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetAbilitySystemComponent"))
	class UScWASC_Character* GetCharacterASC() const { return CharacterASC; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterMesh, GetMesh, GetCharacterMesh, GetFirstPerson", DisplayName = "Get ScW Character Mesh (FirstPerson)"))
	class UScWCharacterMesh_FirstPerson* GetScWFirstPersonCharacterMesh() const { return ScWFirstPersonCharacterMesh; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetAnimInstance, GetFirstPerson", DisplayName = "Get ScW Anim Instance (FirstPerson)"))
	class UScWAnimInstance_FirstPerson* GetScWFirstPersonAnimInstance() const;

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterMesh, GetMesh, GetCharacterMesh, GetThirdPerson", DisplayName = "Get ScW Character Mesh (ThirdPerson)"))
	class UScWCharacterMesh_ThirdPerson* GetScWThirdPersonCharacterMesh() const { return ScWThirdPersonCharacterMesh; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetAnimInstance, GetThirdPerson", DisplayName = "Get ScW Anim Instance (FirstPerson)"))
	class UScWAnimInstance_ThirdPerson* GetScWThirdPersonAnimInstance() const;

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterMovementComponent, GetCMC", DisplayName = "Get ScW Character Movement"))
	class UScWCharacterMovement* GetScWCharacterMovement() const { return ScWCharacterMovement; }

	UFUNCTION(Category = "Components", BlueprintCallable, meta = (KeyWords = "GetCharacterCapsuleComponent, GetCapsule", DisplayName = "Get ScW Character Capsule"))
	class UScWCharacterCapsule* GetScWCharacterCapsule() const { return ScWCharacterCapsule; }

protected:

	UPROPERTY(Category = "Components", BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class UScWASC_Character> CharacterASC;

	UPROPERTY(Category = "Components", BlueprintReadOnly)
	TObjectPtr<class UScWCharacterMesh_FirstPerson> ScWFirstPersonCharacterMesh;

	UPROPERTY(Category = "Components", BlueprintReadOnly)
	TObjectPtr<class UScWCharacterMesh_ThirdPerson> ScWThirdPersonCharacterMesh;

	UPROPERTY(Category = "Components", BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class UScWCharacterMovement> ScWCharacterMovement;

	UPROPERTY(Category = "Components", BlueprintReadOnly, VisibleAnywhere)
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
	
//~ Begin View
public:
	virtual void CalcCamera(float InDeltaSeconds, FMinimalViewInfo& InOutResult) override; // AActor
	virtual bool HasActiveCameraComponent(bool bInForceFindCamera = false) const override; // AActor
	virtual bool HasActivePawnControlCameraComponent() const override; // AActor

	UFUNCTION(Category = "View", BlueprintCallable)
	bool IsInFirstPersonView() const { return bIsInFirstPersonView; }

	UFUNCTION(Category = "View", BlueprintCallable)
	void SetInFirstPersonView(const bool bInIsInFirstPersonView);

	UPROPERTY(Category = "View", BlueprintAssignable)
	FBoolSignature OnIsInFirstPersonViewChangedDelegate;

protected:

	UFUNCTION()
	virtual void HandleSetInFirstPersonViewChanged(bool bInIsInFirstPersonView);

	UPROPERTY(Transient)
	bool bIsInFirstPersonView;

	UPROPERTY(Transient)
	TObjectPtr<UCameraComponent> CachedCameraComponent;

	UPROPERTY(Transient)
	TObjectPtr<USpringArmComponent> CachedCameraSpringArmComponent;
//~ End View

//~ Begin Attributes
protected:

	UFUNCTION()
	void OnDied();

	UPROPERTY(Category = "Attributes", BlueprintAssignable)
	FDefaultEventSignature OnDiedDelegate;
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
	void InputHandheldSwitchScrollPressed();
	void InputHandheldSwitchScrollReleased();
	void InputHandheldSwitchDirectPressed();
	void InputHandheldSwitchDirectReleased();

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
	TObjectPtr<UInputAction> HandheldSwitchScrollAction;

	UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> HandheldSwitchDirectAction;
//~ End Input
	
//~ Begin Team
public:

	UFUNCTION(Category = "Team", BlueprintCallable)
	const FName& GetTeamName(const bool bInGetDefaultFromDataAsset = false) const;

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

//~ Begin Handheld
public:

	UFUNCTION(Category = "Handheld", BlueprintCallable, meta = (KeyWords = "GetCurrentHandheld, GetWeapon, GetCurrentWeapon"))
	class AScWHandheld* GetHandheld() const { return Handheld; }

	template<class InClass>
	InClass* GetHandheld() const { return Cast<InClass>(GetHandheld()); }

	UFUNCTION(Category = "Handheld", BlueprintCallable, meta = (KeyWords = "GetAnimInstance, GetHandheld", DisplayName = "Get ScW Anim Instance (Handheld)"))
	class UScWAnimInstance_Handheld* GetScWHandheldAnimInstance() const;

	UFUNCTION(Category = "Handheld", BlueprintCallable, meta = (KeyWords = "GiveWeapon"))
	class AScWHandheld* GiveHandheld(class UScWHandheldData* InHandheldData, const bool bInDropPrevious = true);

	UFUNCTION(Category = "Handheld", BlueprintCallable, meta = (KeyWords = "DropWeapon"))
	void DropHandheld() { GiveHandheld(nullptr, true); }

	UFUNCTION(Category = "Handheld", BlueprintCallable, meta = (KeyWords = "RemoveWeapon"))
	void RemoveHandheld() { GiveHandheld(nullptr, false); }

	UPROPERTY(Category = "Handheld", BlueprintAssignable, meta = (KeyWords = "OnWeaponChanged"))
	FScWHandheldSignature OnHandheldChanged;

protected:
	virtual void UpdateHandheldAttachment();

	UPROPERTY(Category = "Handheld", BlueprintReadOnly, meta = (KeyWords = "OnWeaponChanged"))
	TObjectPtr<class AScWHandheld> Handheld;

	UPROPERTY(Category = "Handheld", BlueprintReadOnly, meta = (KeyWords = "WeaponAbilitiesHandleArray"))
	TArray<FGameplayAbilitySpecHandle> HandheldAbilitiesHandleArray;
//~ End Handheld

//~ Begin Analytics
public:

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Default Analytics Character Name"))
	FString BP_GetDefaultAnalyticsCharacterName() const;

	UFUNCTION(Category = "Analytics", BlueprintNativeEvent, BlueprintCallable)
	void BP_SendDefaultDesignAnalyticsEvent(const FString& InEvent, const FGACustomFields InAdditionalFields = FGACustomFields()) const;

	UPROPERTY(Category = "Analytics", EditAnywhere, BlueprintReadWrite)
	bool bSendDiedDesignEvent;
//~ End Analytics
};
