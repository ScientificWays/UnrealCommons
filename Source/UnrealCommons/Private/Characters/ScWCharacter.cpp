// Scientific Ways

#include "Characters/ScWCharacter.h"

#include "AI/ScWAIController.h"

#include "Characters/ScWCMC_Base.h"
#include "Characters/DataAssets/ScWCharacterData.h"

#include "Framework/ScWGameState.h"

#include "Gameplay/ScWWeapon_Base.h"
#include "Gameplay/ScWASC_Character.h"
#include "Gameplay/DataAssets/ScWWeaponData_Base.h"

AScWCharacter::AScWCharacter(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer.SetDefaultSubobjectClass<UScWCMC_Base>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AScWAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BaseCMC = Cast<UScWCMC_Base>(GetCharacterMovement());

	CharacterASC = CreateDefaultSubobject<UScWASC_Character>(TEXT("CharacterASC"));
	CharacterASC->SetIsReplicated(true);
	CharacterASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

//~ Begin Statics
AScWCharacter* AScWCharacter::SpawnCharacter(const UObject* InWCO, UScWCharacterData* InData, FTransform InTransform)
{
	if (!InWCO || !InData)
	{
		return nullptr;
	}
	UWorld* World = InWCO->GetWorld();
	if (!World)
	{
		return nullptr;
	}
	AScWCharacter* OutCharacter = World->SpawnActorDeferred<AScWCharacter>(InData->CharacterClass, InTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (OutCharacter)
	{
		OutCharacter->DataAsset = InData;
		OutCharacter->FinishSpawning(InTransform);
	}
	return OutCharacter;
}
//~ End Statics

//~ Begin Initialize
void AScWCharacter::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	if (UWorld* World = GetWorld())
	{
		if (World->IsEditorWorld() && DataAsset)
		{
			DataAsset->K2_UpdateCharacterFromDataAsset(this);
		}
	}
}

void AScWCharacter::BeginPlay() // AActor
{
	if (const AScWGameState* GameState = AScWGameState::TryGetScWGameState(this))
	{
		DataAsset = GameState->K2_GetDataAssetForNewCharacter(this);
	}
	if (DataAsset)
	{
		DataAsset->K2_UpdateCharacterFromDataAsset(this);
	}
	Super::BeginPlay();

	GiveWeapon(DataAsset->DefaultWeaponData);
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
	if (BaseCMC && BaseCMC->IsA(InComponentClass))
	{
		return BaseCMC;
	}
	return Super::FindComponentByClass(InComponentClass);
}

UAbilitySystemComponent* AScWCharacter::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	return CharacterASC;
}

//~ End Components

//~ Begin Controller
void AScWCharacter::PossessedBy(AController* InController) // APawn
{
	Super::PossessedBy(InController);

	if (DataAsset)
	{
		DataAsset->K2_UpdateCharacterFromDataAsset(this);
	}
}
//~ End Controller

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
AScWWeapon_Base* AScWCharacter::GiveWeapon(UScWWeaponData_Base* InWeaponData)
{
	AScWWeapon_Base* PrevWeapon = Weapon;

	CharacterASC->ClearAbilities(WeaponAbilitiesHandleArray, true);

	if (InWeaponData)
	{
		if (Weapon)
		{
			Weapon->Destroy();
		}
		Weapon = AScWWeapon_Base::SpawnWeapon(this, InWeaponData);

		if (Weapon)
		{
			CharacterASC->GiveAbilitiesWithLevels(InWeaponData->WeaponAbilities, WeaponAbilitiesHandleArray);
		}
	}
	else
	{
		Weapon = nullptr;
	}
	if (Weapon != PrevWeapon)
	{
		OnWeaponChanged.Broadcast();
	}
	return Weapon;
}
//~ End Weapon