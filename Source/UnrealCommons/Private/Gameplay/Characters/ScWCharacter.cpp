// Scientific Ways

#include "Gameplay/Characters/ScWCharacter.h"

#include "AI/ScWAIController.h"

#include "Framework/ScWGameState.h"

#include "Gameplay/ScWASC_Character.h"
#include "Gameplay/ScWTypes_Gameplay.h"
#include "Gameplay/Weapons/ScWWeapon_Base.h"
#include "Gameplay/ScWGameplayFunctionLibrary.h"
#include "Gameplay/Weapons/ScWWeaponData_Base.h"
#include "Gameplay/Characters/ScWCharacterMesh.h"
#include "Gameplay/Characters/ScWCharacterData.h"
#include "Gameplay/Characters/ScWCharacterCapsule.h"
#include "Gameplay/Characters/ScWCharacterMovement.h"

AScWCharacter::AScWCharacter(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer
		.SetDefaultSubobjectClass<UScWCharacterMesh>(ACharacter::MeshComponentName)
		.SetDefaultSubobjectClass<UScWCharacterMovement>(ACharacter::CharacterMovementComponentName)
		.SetDefaultSubobjectClass<UScWCharacterCapsule>(ACharacter::CapsuleComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AScWAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ScWCharacterMesh = Cast<UScWCharacterMesh>(GetMesh());
	ScWCharacterMovement = Cast<UScWCharacterMovement>(GetCharacterMovement());
	ScWCharacterCapsule = Cast<UScWCharacterCapsule>(GetCapsuleComponent());

	CharacterASC = CreateDefaultSubobject<UScWASC_Character>(TEXT("CharacterASC"));
	CharacterASC->SetIsReplicated(true);
	CharacterASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	bHorizontalMovementAbsolute = false;
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
		GiveWeapon(DataAsset->DefaultWeaponData);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, GetName() + " has invalid DataAsset on BeginPlay()!", true, true, FLinearColor::Red, 30.0f);
	}
}

void AScWCharacter::EndPlay(const EEndPlayReason::Type InReason) // AActor
{
	if (Weapon)
	{
		Weapon->Destroy();
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
	if (ScWCharacterMesh && ScWCharacterMesh->IsA(InComponentClass))
	{
		return ScWCharacterMesh;
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
//~ End Controller

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
	bool bDestroyActor = true;

	if (DataAsset)
	{
		if (DataAsset->bRagdollOnDeath)
		{
			bDestroyActor = false;
		}
		else if (DataAsset->DiedAnimInstanceClass)
		{
			bDestroyActor = false;
		}
	}
	if (Weapon)
	{
		if (UScWWeaponData_Base* WeaponDataAsset = Weapon->GetDataAsset())
		{
			if (WeaponDataAsset->bDropOnDeath)
			{
				DropWeapon();
			}
			else
			{
				RemoveWeapon();
			}
		}
	}
	if (bDestroyActor)
	{
		Destroy();
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
	ensure(HorizontalMovementAction);
	if (HorizontalMovementAction)
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
	if (ShoveAction)
	{
		EnhancedInputComponent->BindAction(ShoveAction, ETriggerEvent::Started, this, &AScWCharacter::InputShovePressed);
		EnhancedInputComponent->BindAction(ShoveAction, ETriggerEvent::Completed, this, &AScWCharacter::InputShoveReleased);
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
	if (SpecialAction)
	{
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Started, this, &AScWCharacter::InputSpecialPressed);
		EnhancedInputComponent->BindAction(SpecialAction, ETriggerEvent::Completed, this, &AScWCharacter::InputSpecialReleased);
	}
	if (WeaponSwitchScrollAction)
	{
		EnhancedInputComponent->BindAction(WeaponSwitchScrollAction, ETriggerEvent::Started, this, &AScWCharacter::InputWeaponSwitchScrollPressed);
		EnhancedInputComponent->BindAction(WeaponSwitchScrollAction, ETriggerEvent::Completed, this, &AScWCharacter::InputWeaponSwitchScrollPressed);
	}
	if (WeaponSwitchDirectAction)
	{
		EnhancedInputComponent->BindAction(WeaponSwitchDirectAction, ETriggerEvent::Started, this, &AScWCharacter::InputWeaponSwitchDirectPressed);
		EnhancedInputComponent->BindAction(WeaponSwitchDirectAction, ETriggerEvent::Completed, this, &AScWCharacter::InputWeaponSwitchDirectPressed);
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

void AScWCharacter::InputShovePressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Shove));
}

void AScWCharacter::InputShoveReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Shove));
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

void AScWCharacter::InputSpecialPressed()
{
	CharacterASC->PressInputID(static_cast<int32>(EScWAbilityInputID::Special));
}

void AScWCharacter::InputSpecialReleased()
{
	CharacterASC->ReleaseInputID(static_cast<int32>(EScWAbilityInputID::Special));
}

void AScWCharacter::InputWeaponSwitchScrollPressed()
{
	
}

void AScWCharacter::InputWeaponSwitchScrollReleased()
{
	
}

void AScWCharacter::InputWeaponSwitchDirectPressed()
{
	
}

void AScWCharacter::InputWeaponSwitchDirectReleased()
{
	
}
//~ End Input

//~ Begin Team
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

//~ Begin Weapon
AScWWeapon_Base* AScWCharacter::GiveWeapon(UScWWeaponData_Base* InWeaponData, const bool bInDropPrevious)
{
	AScWWeapon_Base* PrevWeapon = Weapon;
	Weapon = nullptr;

	CharacterASC->ClearAbilities(WeaponAbilitiesHandleArray, true);

	if (PrevWeapon)
	{
		UScWWeaponData_Base* WeaponDataAsset = PrevWeapon->GetDataAsset();
		if (bInDropPrevious && WeaponDataAsset && WeaponDataAsset->bCanDrop)
		{
			PrevWeapon->HandleDrop();
		}
		else
		{
			PrevWeapon->Destroy();
		}
	}
	if (InWeaponData)
	{
		Weapon = AScWWeapon_Base::SpawnWeapon(this, InWeaponData);

		if (Weapon)
		{
			CharacterASC->GiveAbilitiesFromGiveData(InWeaponData->WeaponAbilitiesGiveData, WeaponAbilitiesHandleArray);
		}
	}
	if (Weapon != PrevWeapon)
	{
		OnWeaponChanged.Broadcast();
	}
	return Weapon;
}
//~ End Weapon