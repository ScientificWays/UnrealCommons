// G.o.o.P Studios

#include "GAS/ATAASC_Character.h"

#include "Characters/ATACMC_Base.h"

#include "GAS/ATAAS_Character.h"
#include "GAS/ATAGameplayTags.h"

UATAASC_Character::UATAASC_Character()
{
	AttributeSetClass = UATAAS_Character::StaticClass();
}

//~ Begin Statics
UATAASC_Character* UATAASC_Character::TryGetCharacterAtaASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface)
{
	return Cast<UATAASC_Character>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor, bInTryFindComponentIfNoInterface));
}
//~ End Statics

//~ Begin Initialize
void UATAASC_Character::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) // UAbilitySystemComponent
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		return;
	}
	OwnerCMC = InAvatarActor->FindComponentByClass<UATACMC_Base>();

	if (!OwnerCMC)
	{
		OwnerCMC = InOwnerActor->FindComponentByClass<UATACMC_Base>();
	}
	ensure(OwnerCMC);
}

void UATAASC_Character::OnRegister() // UActorComponent
{
	Super::OnRegister();

	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		return;
	}
	const UATAAS_Character* CharacterAS = GetAttributeSet<UATAAS_Character>();
	ensure(CharacterAS);

	if (!CharacterAS)
	{
		return;
	}
	MaxWalkSpeedChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(CharacterAS->GetMaxWalkSpeedAttribute()).AddUObject(this, &ThisClass::OnMaxWalkSpeedChanged);
	MaxWalkSpeedCrouchedChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(CharacterAS->GetMaxWalkSpeedCrouchedAttribute()).AddUObject(this, &ThisClass::OnMaxWalkSpeedCrouchedChanged);
	MaxAccelerationChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(CharacterAS->GetMaxAccelerationAttribute()).AddUObject(this, &ThisClass::OnMaxAccelerationChanged);
	BrakingDecelerationWalkingChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(CharacterAS->GetBrakingDecelerationWalkingAttribute()).AddUObject(this, &ThisClass::OnBrakingDecelerationWalkingChanged);
	GravityScaleChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(CharacterAS->GetGravityScaleAttribute()).AddUObject(this, &ThisClass::OnGravityScaleChanged);
	AirControlChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(CharacterAS->GetAirControlAttribute()).AddUObject(this, &ThisClass::OnAirControlChanged);

	RegisterGameplayTagEvent(FATAGameplayTags::MovementMode, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnMovementModeTagChanged);
}
//~ End Initialize

//~ Begin Attributes
void UATAASC_Character::OnMaxWalkSpeedChanged(const FOnAttributeChangeData& InData)
{
	if (OwnerCMC)
	{
		OwnerCMC->MaxWalkSpeed = InData.NewValue;
	}
}

void UATAASC_Character::OnMaxWalkSpeedCrouchedChanged(const FOnAttributeChangeData& InData)
{
	if (OwnerCMC)
	{
		OwnerCMC->MaxWalkSpeedCrouched = InData.NewValue;
	}
}

void UATAASC_Character::OnMaxAccelerationChanged(const FOnAttributeChangeData& InData)
{
	if (OwnerCMC)
	{
		OwnerCMC->MaxAcceleration = InData.NewValue;
	}
}

void UATAASC_Character::OnBrakingDecelerationWalkingChanged(const FOnAttributeChangeData& InData)
{
	if (OwnerCMC)
	{
		OwnerCMC->BrakingDecelerationWalking = InData.NewValue;
	}
}

void UATAASC_Character::OnGravityScaleChanged(const FOnAttributeChangeData& InData)
{
	if (OwnerCMC)
	{
		OwnerCMC->GravityScale = InData.NewValue;
	}
}

void UATAASC_Character::OnAirControlChanged(const FOnAttributeChangeData& InData)
{
	if (OwnerCMC)
	{
		OwnerCMC->AirControl = InData.NewValue;
	}
}
//~ End Attributes

//~ Begin Tags
void UATAASC_Character::OnMovementModeTagChanged(const FGameplayTag InCallbackTag, int32 InNewNum)
{
	UpdateCharacterMovementModeFromTags();
}

void UATAASC_Character::UpdateCharacterMovementModeFromTags()
{
	if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_None))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_None);
	}
	else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_Walking))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_NavWalking))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_NavWalking);
	}
	else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_Falling))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_Swimming))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Swimming);
	}
	else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_Flying))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Flying);
	}
	/*else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_Custom_Ladder))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(EIDCustomMovementMode::Ladder));
	}
	else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_Custom_Cling))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(EIDCustomMovementMode::Cling));
	}
	else if (HasMatchingGameplayTag(FATAGameplayTags::MovementMode_Custom_InVehicle))
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(EIDCustomMovementMode::InVehicle));
	}*/
	else
	{
		OwnerCMC->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}
//~ End Tags
