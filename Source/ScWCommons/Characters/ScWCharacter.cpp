// G.o.o.P Studios

#include "Characters/ATACharacter.h"

#include "AI/ATAAIController_Base.h"

#include "Characters/ATACMC_Base.h"
#include "Characters/DataAssets/CharacterData.h"

#include "GAS/ATAWeapon_Base.h"
#include "GAS/ATAASC_Character.h"
#include "GAS/DataAssets/WeaponData_Base.h"

#include "World/ATAGameState_Base.h"

AATACharacter::AATACharacter(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer.SetDefaultSubobjectClass<UATACMC_Base>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AATAAIController_Base::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BaseCMC = Cast<UATACMC_Base>(GetCharacterMovement());

	CharacterASC = CreateDefaultSubobject<UATAASC_Character>(TEXT("CharacterASC"));
	CharacterASC->SetIsReplicated(true);
	CharacterASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

//~ Begin Statics
AATACharacter* AATACharacter::SpawnCharacter(const UObject* InWCO, UCharacterData* InData, FTransform InTransform)
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
	AATACharacter* OutCharacter = World->SpawnActorDeferred<AATACharacter>(InData->CharacterClass, InTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (OutCharacter)
	{
		OutCharacter->DataAsset = InData;
		OutCharacter->FinishSpawning(InTransform);
	}
	return OutCharacter;
}
//~ End Statics

//~ Begin Initialize
void AATACharacter::OnConstruction(const FTransform& InTransform) // AActor
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

void AATACharacter::BeginPlay() // AActor
{
	if (const AATAGameState_Base* GameState = AATAGameState_Base::TryGetAtaGameState(this))
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

void AATACharacter::EndPlay(const EEndPlayReason::Type InReason) // AActor
{
	if (Weapon)
	{
		Weapon->Destroy();
	}
	Super::EndPlay(InReason);
}
//~ End Initialize

//~ Begin Components
UActorComponent* AATACharacter::FindComponentByClass(const TSubclassOf<UActorComponent> InComponentClass) const
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

UAbilitySystemComponent* AATACharacter::GetAbilitySystemComponent() const // IAbilitySystemInterface
{
	return CharacterASC;
}

//~ End Components

//~ Begin Controller
void AATACharacter::PossessedBy(AController* InController) // APawn
{
	Super::PossessedBy(InController);

	if (DataAsset)
	{
		DataAsset->K2_UpdateCharacterFromDataAsset(this);
	}
}
//~ End Controller

//~ Begin Team
FGenericTeamId AATACharacter::GetGenericTeamId() const // IGenericTeamAgentInterface
{
	if (IGenericTeamAgentInterface* TeamAgentInterface = GetController<IGenericTeamAgentInterface>())
	{
		return TeamAgentInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

void AATACharacter::SetGenericTeamId(const FGenericTeamId& InNewTeamId) // IGenericTeamAgentInterface
{
	if (IGenericTeamAgentInterface* TeamAgentInterface = GetController<IGenericTeamAgentInterface>())
	{
		TeamAgentInterface->SetGenericTeamId(InNewTeamId);
	}
}
//~ End Team

//~ Begin GameplayTags
void AATACharacter::GetOwnedGameplayTags(FGameplayTagContainer& OutTagContainer) const // IGameplayTagAssetInterface
{
	if (CharacterASC)
	{
		CharacterASC->GetOwnedGameplayTags(OutTagContainer);
	}
}

bool AATACharacter::HasMatchingGameplayTag(FGameplayTag InTagToCheck) const // IGameplayTagAssetInterface
{
	return CharacterASC ? CharacterASC->HasMatchingGameplayTag(InTagToCheck) : false;
}

bool AATACharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& InTagContainer) const // IGameplayTagAssetInterface
{
	return CharacterASC ? CharacterASC->HasAllMatchingGameplayTags(InTagContainer) : false;
}

bool AATACharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& InTagContainer) const // IGameplayTagAssetInterface
{
	return CharacterASC ? CharacterASC->HasAnyMatchingGameplayTags(InTagContainer) : false;
}
//~ End GameplayTags

//~ Begin GameplayCue
void AATACharacter::HandleGameplayCue(UObject* InSelf, FGameplayTag InGameplayCueTag, EGameplayCueEvent::Type InEventType, const FGameplayCueParameters& InParameters) // IGameplayCueInterface
{

}
//~ End GameplayCue

//~ Begin Weapon
AATAWeapon_Base* AATACharacter::GiveWeapon(UWeaponData_Base* InWeaponData)
{
	AATAWeapon_Base* PrevWeapon = Weapon;

	CharacterASC->ClearAbilities(WeaponAbilitiesHandleArray, true);

	if (InWeaponData)
	{
		if (Weapon)
		{
			Weapon->Destroy();
		}
		Weapon = AATAWeapon_Base::SpawnWeapon(this, InWeaponData);

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