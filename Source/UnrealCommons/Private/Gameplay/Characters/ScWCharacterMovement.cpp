// Scientific Ways

#include "Gameplay/Characters/ScWCharacterMovement.h"

#include "Gameplay/ScWASC_Base.h"
#include "Gameplay/ScWDamageType.h"
#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/Attributes/ScWAS_Character.h"

UScWCharacterMovement::UScWCharacterMovement()
{
	//bOrientRotationToMovement = true;
	//RotationRate = FRotator(0.0, 720.0, 0.0);
}

//~ Begin Initialize
void UScWCharacterMovement::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);

	
}

void UScWCharacterMovement::InitFromASC(UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) // IScWASC_InitInterface
{
	ensureReturn(InInitASC);
	//ensureReturn(InOwnerActor);
	//ensureReturn(InAvatarActor);

	const UScWAS_Character* CharacterAS = InInitASC->GetAttributeSet<UScWAS_Character>();
	ensureReturn(CharacterAS);

	MaxWalkSpeedChangedDelegateHandle = InInitASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetMaxWalkSpeedAttribute()).AddUObject(this, &ThisClass::OnMaxWalkSpeedAttributeChanged);
	MaxWalkSpeedCrouchedChangedDelegateHandle = InInitASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetMaxWalkSpeedCrouchedAttribute()).AddUObject(this, &ThisClass::OnMaxWalkSpeedCrouchedAttributeChanged);
	MaxAccelerationChangedDelegateHandle = InInitASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetMaxAccelerationAttribute()).AddUObject(this, &ThisClass::OnMaxAccelerationAttributeChanged);
	BrakingDecelerationWalkingChangedDelegateHandle = InInitASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetBrakingDecelerationWalkingAttribute()).AddUObject(this, &ThisClass::OnBrakingDecelerationWalkingAttributeChanged);
	GravityScaleChangedDelegateHandle = InInitASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetGravityScaleAttribute()).AddUObject(this, &ThisClass::OnGravityScaleAttributeChanged);
	AirControlChangedDelegateHandle = InInitASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetAirControlAttribute()).AddUObject(this, &ThisClass::OnAirControlAttributeChanged);
	MassChangedDelegateHandle = InInitASC->GetGameplayAttributeValueChangeDelegate(CharacterAS->GetMassAttribute()).AddUObject(this, &ThisClass::OnMassAttributeChanged);

	InInitASC->RegisterGameplayTagEvent(FScWGameplayTags::MovementMode, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnMovementModeTagNumChanged);

	InInitASC->OnDamageApplied.AddDynamic(this, &ThisClass::HandleDamageApplied);
	InInitASC->OnDied.AddDynamic(this, &ThisClass::HandleDied);
}
//~ End Initialize

//~ Begin Rotation
void UScWCharacterMovement::PhysicsRotation(float InDeltaTime) // UCharacterMovementComponent
{
	/*if (PhysicsRotationBlockSourceSet.IsEmpty())
	{
		Super::PhysicsRotation(InDeltaTime);
	}
	else
	{
		return;
	}*/
	Super::PhysicsRotation(InDeltaTime);
}
//~ End Rotation

//~ Begin Attributes
void UScWCharacterMovement::OnMaxWalkSpeedAttributeChanged(const FOnAttributeChangeData& InData)
{
	MaxWalkSpeed = InData.NewValue;
}

void UScWCharacterMovement::OnMaxWalkSpeedCrouchedAttributeChanged(const FOnAttributeChangeData& InData)
{
	MaxWalkSpeedCrouched = InData.NewValue;
}

void UScWCharacterMovement::OnMaxAccelerationAttributeChanged(const FOnAttributeChangeData& InData)
{
	MaxAcceleration = InData.NewValue;
}

void UScWCharacterMovement::OnBrakingDecelerationWalkingAttributeChanged(const FOnAttributeChangeData& InData)
{
	BrakingDecelerationWalking = InData.NewValue;
}

void UScWCharacterMovement::OnGravityScaleAttributeChanged(const FOnAttributeChangeData& InData)
{
	GravityScale = InData.NewValue;
}

void UScWCharacterMovement::OnAirControlAttributeChanged(const FOnAttributeChangeData& InData)
{
	AirControl = InData.NewValue;
}

void UScWCharacterMovement::OnMassAttributeChanged(const FOnAttributeChangeData& InData)
{
	Mass = InData.NewValue;
}
//~ End Attributes

//~ Begin Tags
void UScWCharacterMovement::OnMovementModeTagNumChanged(const FGameplayTag InCallbackTag, int32 InNewNum)
{
	UpdateCharacterMovementModeFromTags();
}

void UScWCharacterMovement::UpdateCharacterMovementModeFromTags()
{
	UScWASC_Base* InitASC = GetInitASC();
	ensureReturn(InitASC);

	if (InitASC->HasMatchingGameplayTag(FScWGameplayTags::MovementMode_None))
	{
		SetMovementMode(EMovementMode::MOVE_None);
	}
	else if (InitASC->HasMatchingGameplayTag(FScWGameplayTags::MovementMode_Walking))
	{
		SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else if (InitASC->HasMatchingGameplayTag(FScWGameplayTags::MovementMode_NavWalking))
	{
		SetMovementMode(EMovementMode::MOVE_NavWalking);
	}
	else if (InitASC->HasMatchingGameplayTag(FScWGameplayTags::MovementMode_Falling))
	{
		SetMovementMode(EMovementMode::MOVE_Falling);
	}
	else if (InitASC->HasMatchingGameplayTag(FScWGameplayTags::MovementMode_Swimming))
	{
		SetMovementMode(EMovementMode::MOVE_Swimming);
	}
	else if (InitASC->HasMatchingGameplayTag(FScWGameplayTags::MovementMode_Flying))
	{
		SetMovementMode(EMovementMode::MOVE_Flying);
	}
	else
	{
		SetDefaultMovementMode();
	}
}
//~ End Tags

//~ Begin Damage
void UScWCharacterMovement::HandleDamageApplied(float InDamage, const FReceivedDamageData& InData)
{
	UScWASC_Base* InitASC = GetInitASC();
	ensureReturn(InitASC);

	const FReceivedDamageData& LastAppliedDamageData = InitASC->GetLastAppliedDamageData();
	ensureReturn(LastAppliedDamageData.DamageType);

	LastAppliedDamageData.DamageType->ApplyImpactImpulseToCharacterMovement(this, LastAppliedDamageData.HitResult);
}

void UScWCharacterMovement::HandleDied()
{
	DisableMovement();
}
//~ End Damage
