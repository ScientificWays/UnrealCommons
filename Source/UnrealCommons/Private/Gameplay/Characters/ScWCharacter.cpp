// Scientific Ways

#include "Gameplay/Characters/ScWCharacter.h"

#include "AI/ScWAIController.h"

#include "Framework/ScWGameState.h"

#include "Gameplay/ScWASC_Character.h"
#include "Gameplay/ScWTypes_Gameplay.h"
#include "Gameplay/Handhelds/ScWHandheld.h"
#include "Gameplay/Handhelds/ScWHandheldData.h"
#include "Gameplay/ScWGameplayFunctionLibrary.h"
#include "Gameplay/Characters/ScWCharacterData.h"
#include "Gameplay/Characters/ScWCharacterCapsule.h"
#include "Gameplay/Characters/ScWCharacterMovement.h"
#include "Gameplay/Characters/ScWCharacterMesh_FirstPerson.h"
#include "Gameplay/Characters/ScWCharacterMesh_ThirdPerson.h"

AScWCharacter::AScWCharacter(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer
		.SetDefaultSubobjectClass<UScWCharacterMesh_ThirdPerson>(ACharacter::MeshComponentName)
		.SetDefaultSubobjectClass<UScWCharacterMovement>(ACharacter::CharacterMovementComponentName)
		.SetDefaultSubobjectClass<UScWCharacterCapsule>(ACharacter::CapsuleComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AScWAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ScWThirdPersonCharacterMesh = Cast<UScWCharacterMesh_ThirdPerson>(GetMesh());
	ScWCharacterMovement = Cast<UScWCharacterMovement>(GetCharacterMovement());
	ScWCharacterCapsule = Cast<UScWCharacterCapsule>(GetCapsuleComponent());

	CharacterASC = CreateDefaultSubobject<UScWASC_Character>(TEXT("CharacterASC"));
	CharacterASC->SetIsReplicated(true);
	CharacterASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	bHorizontalMovementAbsolute = false;

	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputMappingContext, DefaultInputMappingContext, "/UnrealCommons/Blueprints/Input/IMC_CommonPlayerCharacter.IMC_CommonPlayerCharacter");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, HorizontalMovementAction, "/UnrealCommons/Blueprints/Input/IA_HorizontalMovement.IA_HorizontalMovement");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, UsePrimaryAction, "/UnrealCommons/Blueprints/Input/IA_UsePrimary.IA_UsePrimary");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, UseSecondaryAction, "/UnrealCommons/Blueprints/Input/IA_UseSecondary.IA_UseSecondary");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, AbilityConfirmAction, "/UnrealCommons/Blueprints/Input/IA_UsePrimary.IA_UsePrimary");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, AbilityCancelAction, "/UnrealCommons/Blueprints/Input/IA_UseSecondary.IA_UseSecondary");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, JumpAction, "/UnrealCommons/Blueprints/Input/IA_Jump.IA_Jump");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, CrouchAction, "/UnrealCommons/Blueprints/Input/IA_Crouch.IA_Crouch");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, InteractAction, "/UnrealCommons/Blueprints/Input/IA_Interact.IA_Interact");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, ReloadAction, "/UnrealCommons/Blueprints/Input/IA_Reload.IA_Reload");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, SprintAction, "/UnrealCommons/Blueprints/Input/IA_Sprint.IA_Sprint");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, WalkAction, "/UnrealCommons/Blueprints/Input/IA_Walk.IA_Walk");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, DropAction, "/UnrealCommons/Blueprints/Input/IA_Drop.IA_Drop");
	CONSTRUCTOR_TRY_LOAD_OBJECT(UInputAction, FlashlightAction, "/UnrealCommons/Blueprints/Input/IA_Flashlight.IA_Flashlight");
}

//~ Begin Statics
AScWCharacter* AScWCharacter::SpawnCharacter(const UObject* InWCO, UScWCharacterData* InData, FTransform InTransform)
{
	ensureReturn(InWCO, nullptr);
	ensureReturn(InData, nullptr);
	
	UWorld* World = InWCO->GetWorld();
	ensureReturn(World, nullptr);

	AScWCharacter* OutCharacter = World->SpawnActorDeferred<AScWCharacter>(InData->CharacterClass, InTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	ensureReturn(OutCharacter, nullptr);
	
	OutCharacter->DataAsset = InData;
	OutCharacter->FinishSpawning(InTransform);
	return OutCharacter;
}
//~ End Statics

//~ Begin Initialize
void AScWCharacter::PostInitializeComponents() // AActor
{
	UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		if (const AScWGameState* GameState = AScWGameState::TryGetScWGameState(this))
		{
			DataAsset = GameState->BP_GetDataAssetForNewCharacter(this);
		}
		if (DataAsset)
		{
			DataAsset->BP_InitializeCharacterController(this);
		}
	}
	Super::PostInitializeComponents();

	ensure((GetController() == nullptr) || (IsPlayerCharacter() == GetController()->IsPlayerController()));

	ForEachComponent(false, [this](UActorComponent* InComponent)
	{
		if (UCameraComponent* SampleCameraComponent = Cast<UCameraComponent>(InComponent))
		{
			ensureReturn(CachedCameraComponent == nullptr);
			CachedCameraComponent = SampleCameraComponent;

			if (USpringArmComponent* SampleSpringArmComponent = Cast<USpringArmComponent>(CachedCameraComponent->GetAttachParent()))
			{
				ensureReturn(CachedCameraSpringArmComponent == nullptr);
				CachedCameraSpringArmComponent = SampleSpringArmComponent;
			}
		}
		else if (UScWCharacterMesh_FirstPerson* SampleFirstPersonMeshComponent = Cast<UScWCharacterMesh_FirstPerson>(InComponent))
		{
			ensureReturn(ScWFirstPersonCharacterMesh == nullptr);
			ScWFirstPersonCharacterMesh = SampleFirstPersonMeshComponent;
		}
	});

	if (World && World->IsGameWorld() && DataAsset)
	{
		DataAsset->BP_InitializeCharacterComponents(this);
	}
}

void AScWCharacter::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	if (!bIsPlayerCharacter && GetName().Contains("Player"))
	{
		UKismetSystemLibrary::PrintString(this, GetName() + " has bIsPlayerCharacter == false!", true, true, FLinearColor::Red, 30.0f);
	}
	if (UWorld* World = GetWorld())
	{
		if (World->IsEditorWorld() && DataAsset)
		{
			DataAsset->BP_InitializeCharacterComponents(this);
		}
	}
}

void AScWCharacter::BeginPlay() // AActor
{
	Super::BeginPlay();

	if (CharacterASC)
	{
		CharacterASC->OnDied.AddDynamic(this, &ThisClass::OnDied);
	}
	if (DataAsset)
	{
		GiveHandheld(DataAsset->DefaultHandheldData);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, GetName() + " has invalid DataAsset on BeginPlay()!", true, true, FLinearColor::Red, 30.0f);
	}
	OnIsInFirstPersonViewChangedDelegate.AddDynamic(this, &ThisClass::HandleSetInFirstPersonViewChanged);
	SetInFirstPersonView(bStartInFirstPersonView);
}

void AScWCharacter::EndPlay(const EEndPlayReason::Type InReason) // AActor
{
	if (Handheld)
	{
		Handheld->Destroy();
	}
	Super::EndPlay(InReason);
}
//~ End Initialize

//~ Begin Components
UActorComponent* AScWCharacter::FindComponentByClass(const TSubclassOf<UActorComponent> InComponentClass) const
{
	if (!InComponentClass)
	{
		return nullptr;
	}
	if (CharacterASC && CharacterASC->IsA(InComponentClass))
	{
		return CharacterASC;
	}
	if (ScWThirdPersonCharacterMesh && ScWThirdPersonCharacterMesh->IsA(InComponentClass))
	{
		return ScWThirdPersonCharacterMesh;
	}
	if (ScWFirstPersonCharacterMesh && ScWFirstPersonCharacterMesh->IsA(InComponentClass))
	{
		return ScWFirstPersonCharacterMesh;
	}
	if (ScWCharacterMovement && ScWCharacterMovement->IsA(InComponentClass))
	{
		return ScWCharacterMovement;
	}
	if (ScWCharacterCapsule && ScWCharacterCapsule->IsA(InComponentClass))
	{
		return ScWCharacterCapsule;
	}
	return Super::FindComponentByClass(InComponentClass);
}

UAbilitySystemComponent* AScWCharacter::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	return CharacterASC;
}

UScWAnimInstance_FirstPerson* AScWCharacter::GetScWFirstPersonAnimInstance() const
{
	ensureReturn(ScWFirstPersonCharacterMesh, nullptr);
	return Cast<UScWAnimInstance_FirstPerson>(ScWFirstPersonCharacterMesh->GetAnimInstance());
}

UScWAnimInstance_ThirdPerson* AScWCharacter::GetScWThirdPersonAnimInstance() const
{
	ensureReturn(ScWThirdPersonCharacterMesh, nullptr);
	return Cast<UScWAnimInstance_ThirdPerson>(ScWThirdPersonCharacterMesh->GetAnimInstance());
}
//~ End Components

//~ Begin Controller
FVector AScWCharacter::GetPawnViewLocation() const // APawn
{
	return Super::GetPawnViewLocation();
}

void AScWCharacter::SpawnDefaultController() // APawn
{
	Super::SpawnDefaultController();


}

void AScWCharacter::PossessedBy(AController* InController) // APawn
{
	Super::PossessedBy(InController);

	if (DataAsset)
	{
		DataAsset->BP_InitializeCharacterController(this);
	}
	if (APlayerController* OwnerPlayerController = Cast<APlayerController>(InController))
	{
		UScWGameplayFunctionLibrary::AddEnhancedInputMappingContextTo(OwnerPlayerController, DefaultInputMappingContext, DefaultInputMappingContextPriority, DefaultInputMappingContextOptions);
	}
}

void AScWCharacter::UnPossessed() // APawn
{
	if (APlayerController* OwnerPlayerController = GetController<APlayerController>())
	{
		UScWGameplayFunctionLibrary::RemoveEnhancedInputMappingContextFrom(OwnerPlayerController, DefaultInputMappingContext, DefaultInputMappingContextOptions);
	}
	Super::UnPossessed();
}

void AScWCharacter::NotifyControllerChanged() // APawn
{
	Super::NotifyControllerChanged();

	ensureReturn(CharacterASC);
	CharacterASC->RefreshAbilityActorInfo();
}
//~ End Controller

//~ Begin View
void AScWCharacter::CalcCamera(float InDeltaSeconds, FMinimalViewInfo& InOutResult) // AActor
{
	if (CachedCameraComponent && CachedCameraComponent->IsActive())
	{
		CachedCameraComponent->GetCameraView(InDeltaSeconds, InOutResult);
	}
	else
	{
		Super::CalcCamera(InDeltaSeconds, InOutResult);
	}
}

bool AScWCharacter::HasActiveCameraComponent(bool bInForceFindCamera) const // AActor
{
	if (CachedCameraComponent && CachedCameraComponent->IsActive())
	{
		return true;
	}
	return Super::HasActiveCameraComponent(bInForceFindCamera);
}

bool AScWCharacter::HasActivePawnControlCameraComponent() const // AActor
{
	if (CachedCameraComponent&& CachedCameraComponent->IsActive() && (CachedCameraComponent->bUsePawnControlRotation || (CachedCameraSpringArmComponent && CachedCameraSpringArmComponent->bUsePawnControlRotation)))
	{
		return true;
	}
	return Super::HasActivePawnControlCameraComponent();
}

void AScWCharacter::SetInFirstPersonView(const bool bInIsInFirstPersonView)
{
	bIsInFirstPersonView = bInIsInFirstPersonView;
	OnIsInFirstPersonViewChangedDelegate.Broadcast(bIsInFirstPersonView);
}

void AScWCharacter::HandleSetInFirstPersonViewChanged(bool bInIsInFirstPersonView)
{
	UpdateHandheldAttachment();
}
//~ End View

//~ Begin Attributes
void AScWCharacter::OnDied()
{
	if (APlayerController* OwnerPlayerController = GetController<APlayerController>())
	{
		UScWGameplayFunctionLibrary::RemoveEnhancedInputMappingContextFrom(OwnerPlayerController, DefaultInputMappingContext, DefaultInputMappingContextOptions);
	}
	else
	{
		DetachFromControllerPendingDestroy();
	}
	bool bDestroyActorNextTick = true;

	if (DataAsset)
	{
		if (DataAsset->bRagdollOnDeath)
		{
			bDestroyActorNextTick = false;
		}
		else if (DataAsset->TP_DiedAnimInstanceClass)
		{
			bDestroyActorNextTick = false;
		}
	}
	if (Handheld)
	{
		if (UScWHandheldData* HandheldDataAsset = Handheld->GetDataAsset())
		{
			if (HandheldDataAsset->bDropOnDeath)
			{
				DropHandheld();
			}
			else
			{
				RemoveHandheld();
			}
		}
	}
	if (bDestroyActorNextTick)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::K2_DestroyActor);
	}
}
//~ End Attributes

//~ Begin Input
void AScWCharacter::SetupPlayerInputComponent(UInputComponent* InInputComponent) // APawn
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InInputComponent);
	ensure(EnhancedInputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}
	ensureIf(HorizontalMovementAction)
	{
		auto HorizontalMovementMethodPtr = bHorizontalMovementAbsolute ? &AScWCharacter::InputHorizontalMovement_Absolute : &AScWCharacter::InputHorizontalMovement;
		EnhancedInputComponent->BindAction(HorizontalMovementAction, ETriggerEvent::Triggered, this, HorizontalMovementMethodPtr);
		EnhancedInputComponent->BindAction(HorizontalMovementAction, ETriggerEvent::Completed, this, HorizontalMovementMethodPtr);
	}
	if (UsePrimaryAction)
	{
		EnhancedInputComponent->BindAction(UsePrimaryAction, ETriggerEvent::Started, this, &AScWCharacter::InputUsePrimaryPressed);
		EnhancedInputComponent->BindAction(UsePrimaryAction, ETriggerEvent::Completed, this, &AScWCharacter::InputUsePrimaryReleased);
	}
	if (UseSecondaryAction)
	{
		EnhancedInputComponent->BindAction(UseSecondaryAction, ETriggerEvent::Started, this, &AScWCharacter::InputUseSecondaryPressed);
		EnhancedInputComponent->BindAction(UseSecondaryAction, ETriggerEvent::Completed, this, &AScWCharacter::InputUseSecondaryReleased);
	}
	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AScWCharacter::InputJumpPressed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AScWCharacter::InputJumpReleased);
	}
	if (CrouchAction)
	{
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AScWCharacter::InputCrouchPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AScWCharacter::InputCrouchReleased);
	}
	if (InteractAction)
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AScWCharacter::InputInteractPressed);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AScWCharacter::InputInteractReleased);
	}
	if (ReloadAction)
	{
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AScWCharacter::InputReloadPressed);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Completed, this, &AScWCharacter::InputReloadReleased);
	}
	if (SprintAction)
	{
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AScWCharacter::InputSprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AScWCharacter::InputSprintReleased);
	}
	if (WalkAction)
	{
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &ThisClass::InputWalkPressed);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AScWCharacter::InputWalkReleased);
	}
	if (DropAction)
	{
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &AScWCharacter::InputDropPressed);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Completed, this, &AScWCharacter::InputDropReleased);
	}
	if (FlashlightAction)
	{
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AScWCharacter::InputFlashlightPressed);
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Completed, this, &AScWCharacter::InputFlashlightReleased);
	}
	if (HandheldSwitchScrollAction)
	{
		EnhancedInputComponent->BindAction(HandheldSwitchScrollAction, ETriggerEvent::Started, this, &AScWCharacter::InputHandheldSwitchScrollPressed);
		EnhancedInputComponent->BindAction(HandheldSwitchScrollAction, ETriggerEvent::Completed, this, &AScWCharacter::InputHandheldSwitchScrollPressed);
	}
	if (HandheldSwitchDirectAction)
	{
		EnhancedInputComponent->BindAction(HandheldSwitchDirectAction, ETriggerEvent::Started, this, &AScWCharacter::InputHandheldSwitchDirectPressed);
		EnhancedInputComponent->BindAction(HandheldSwitchDirectAction, ETriggerEvent::Completed, this, &AScWCharacter::InputHandheldSwitchDirectPressed);
	}
}

void AScWCharacter::InputHorizontalMovement(const FInputActionInstance& InActionInstance)
{
	FVector2D InputValue = InActionInstance.GetValue().Get<FVector2D>();

	float MoveForwardValue = InputValue.Y;
	float MoveRightValue = InputValue.X;

	AddMovementInput(GetActorForwardVector() * MoveForwardValue + GetActorRightVector() * MoveRightValue);
	
	if (MoveForwardValue > 0.0f)
	{
		OnForwardInputTriggeredDelegate.Broadcast();
	}
	else if (MoveForwardValue < 0.0f)
	{
		OnBackwardInputTriggeredDelegate.Broadcast();
	}
	if (MoveRightValue > 0.0f)
	{
		OnRightInputTriggeredDelegate.Broadcast();
	}
	else if (MoveRightValue < 0.0f)
	{
		OnLeftInputTriggeredDelegate.Broadcast();
	}
}

void AScWCharacter::InputHorizontalMovement_Absolute(const FInputActionInstance& InActionInstance)
{
	FVector2D InputValue = InActionInstance.GetValue().Get<FVector2D>();

	float MoveForwardValue = InputValue.Y;
	float MoveRightValue = InputValue.X;

	AddMovementInput(FVector(MoveForwardValue, MoveRightValue, 0.0f));

	if (MoveForwardValue > 0.0f)
	{
		OnForwardInputTriggeredDelegate.Broadcast();
	}
	else if (MoveForwardValue < 0.0f)
	{
		OnBackwardInputTriggeredDelegate.Broadcast();
	}
	if (MoveRightValue > 0.0f)
	{
		OnRightInputTriggeredDelegate.Broadcast();
	}
	else if (MoveRightValue < 0.0f)
	{
		OnLeftInputTriggeredDelegate.Broadcast();
	}
}

void AScWCharacter::InputAbilityConfirmPressed()
{
	CharacterASC->InputConfirm();
}

void AScWCharacter::InputAbilityCancelPressed()
{
	CharacterASC->InputCancel();
}

void AScWCharacter::InputUsePrimaryPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::UsePrimary));
}

void AScWCharacter::InputUsePrimaryReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::UsePrimary));
}

void AScWCharacter::InputUseSecondaryPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::UseSecondary));
}

void AScWCharacter::InputUseSecondaryReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::UseSecondary));
}

void AScWCharacter::InputJumpPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Jump));
}

void AScWCharacter::InputJumpReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Jump));
}

void AScWCharacter::InputCrouchPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Crouch));
}

void AScWCharacter::InputCrouchReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Crouch));
}

void AScWCharacter::InputInteractPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Interact));
}

void AScWCharacter::InputInteractReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Interact));
}

void AScWCharacter::InputReloadPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Reload));
}

void AScWCharacter::InputReloadReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Reload));
}

void AScWCharacter::InputSprintPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Sprint));
}

void AScWCharacter::InputSprintReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Sprint));
}

void AScWCharacter::InputWalkPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Walk));
}

void AScWCharacter::InputWalkReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Walk));
}

void AScWCharacter::InputDropPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Drop));
}

void AScWCharacter::InputDropReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Drop));
}

void AScWCharacter::InputFlashlightPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Flashlight));
}

void AScWCharacter::InputFlashlightReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Flashlight));
}

void AScWCharacter::InputHandheldSwitchScrollPressed()
{
	
}

void AScWCharacter::InputHandheldSwitchScrollReleased()
{
	
}

void AScWCharacter::InputHandheldSwitchDirectPressed()
{
	
}

void AScWCharacter::InputHandheldSwitchDirectReleased()
{
	
}
//~ End Input

//~ Begin Team
const FName& AScWCharacter::GetTeamName(const bool bInGetDefaultFromDataAsset) const
{
	if (bInGetDefaultFromDataAsset)
	{
		ensureReturn(DataAsset, AScWGameState::InvalidTeamName);
		return DataAsset->DefaultTeamName;
	}
	else
	{
		AScWGameState* GameState = AScWGameState::TryGetScWGameState(this);
		ensureReturn(GameState, AScWGameState::InvalidTeamName);

		return GameState->GetTeamName(GetGenericTeamId());
	}
}

FGenericTeamId AScWCharacter::GetGenericTeamId() const // IGenericTeamAgentInterface
{
	if (IGenericTeamAgentInterface* TeamAgentInterface = GetController<IGenericTeamAgentInterface>())
	{
		return TeamAgentInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

void AScWCharacter::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
{
	if (IGenericTeamAgentInterface* TeamAgentInterface = GetController<IGenericTeamAgentInterface>())
	{
		TeamAgentInterface->SetGenericTeamId(InNewTeamId);
	}
}
//~ End Team

//~ Begin GameplayTags
void AScWCharacter::GetOwnedGameplayTags(FGameplayTagContainer& OutTagContainer) const // IGameplayTagAssetInterface
{
	if (CharacterASC)
	{
		CharacterASC->GetOwnedGameplayTags(OutTagContainer);
	}
}

bool AScWCharacter::HasMatchingGameplayTag(FGameplayTag InTagToCheck) const // IGameplayTagAssetInterface
{
	return CharacterASC ? CharacterASC->HasMatchingGameplayTag(InTagToCheck) : false;
}

bool AScWCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& InTagContainer) const // IGameplayTagAssetInterface
{
	return CharacterASC ? CharacterASC->HasAllMatchingGameplayTags(InTagContainer) : false;
}

bool AScWCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& InTagContainer) const // IGameplayTagAssetInterface
{
	return CharacterASC ? CharacterASC->HasAnyMatchingGameplayTags(InTagContainer) : false;
}
//~ End GameplayTags

//~ Begin GameplayCue
void AScWCharacter::HandleGameplayCue(UObject* InSelf, FGameplayTag InGameplayCueTag, EGameplayCueEvent::Type InEventType, const FGameplayCueParameters& InParameters) // IGameplayCueInterface
{

}
//~ End GameplayCue

//~ Begin Handheld
UScWAnimInstance_Handheld* AScWCharacter::GetScWHandheldAnimInstance() const
{
	ensureReturn(Handheld, nullptr);
	return Handheld->GetMeshAnimInstance();
}

AScWHandheld* AScWCharacter::GiveHandheld(UScWHandheldData* InHandheldData, const bool bInDropPrevious)
{
	AScWHandheld* PrevHandheld = Handheld;
	Handheld = nullptr;

	CharacterASC->ClearAbilities(HandheldAbilitiesHandleArray, true);

	if (PrevHandheld)
	{
		UScWHandheldData* PrevHandheldData = PrevHandheld->GetDataAsset();
		if (bInDropPrevious && PrevHandheldData && PrevHandheldData->bCanDrop)
		{
			PrevHandheld->HandleDrop();
		}
		else
		{
			PrevHandheld->Destroy();
		}
	}
	if (InHandheldData)
	{
		Handheld = AScWHandheld::SpawnHandheldFor(this, InHandheldData);

		if (Handheld)
		{
			CharacterASC->GiveAbilitiesFromGiveData(InHandheldData->HandheldAbilitiesGiveData, HandheldAbilitiesHandleArray);
		}
	}
	if (Handheld != PrevHandheld)
	{
		OnHandheldChanged.Broadcast(PrevHandheld, Handheld);
	}
	return Handheld;
}

void AScWCharacter::UpdateHandheldAttachment()
{
	if (Handheld)
	{
		Handheld->UpdateAttachmentToOwner();
	}
}
//~ End Handheld
