// Scientific Ways

#include "Gameplay/ScWWeapon_Melee.h"

#include "Characters/ScWCharacter.h"

#include "Gameplay/DataAssets/ScWWeaponData_Melee.h"

AScWWeapon_Melee::AScWWeapon_Melee(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("MeleeWeapon"), false);
	Collision->SetupAttachment(RootComponent);
}

//~ Begin Initialize
void AScWWeapon_Melee::K2_UpdateFromDataAsset_Implementation() // AScWWeapon_Base
{
	Super::K2_UpdateFromDataAsset_Implementation();

	if (UScWWeaponData_Melee* WeaponDataAsset = Cast<UScWWeaponData_Melee>(DataAsset))
	{
		if (Collision)
		{
			Collision->SetCapsuleSize(WeaponDataAsset->CapsuleRadiusHeight.X, WeaponDataAsset->CapsuleRadiusHeight.Y);
			Collision->SetRelativeTransform(WeaponDataAsset->CapsuleRelativeTransform);
		}
	}
}

void AScWWeapon_Melee::BeginPlay() // AActor
{
	Super::BeginPlay();

	if (Collision)
	{
		Collision->ClearMoveIgnoreActors();
		Collision->IgnoreActorWhenMoving(OwnerCharacter, true);
	}
}
//~ End Initialize
