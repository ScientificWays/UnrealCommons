// G.o.o.P Studios

#include "GAS/ATAWeapon_Melee.h"

#include "Characters/ATACharacter.h"

#include "GAS/DataAssets/WeaponData_Melee.h"

AATAWeapon_Melee::AATAWeapon_Melee(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("MeleeWeapon"), false);
	Collision->SetupAttachment(RootComponent);
}

//~ Begin Initialize
void AATAWeapon_Melee::K2_UpdateFromDataAsset_Implementation() // AATAWeapon_Base
{
	Super::K2_UpdateFromDataAsset_Implementation();

	if (UWeaponData_Melee* WeaponDataAsset = Cast<UWeaponData_Melee>(DataAsset))
	{
		if (Collision)
		{
			Collision->SetCapsuleSize(WeaponDataAsset->CapsuleRadiusHeight.X, WeaponDataAsset->CapsuleRadiusHeight.Y);
			Collision->SetRelativeTransform(WeaponDataAsset->CapsuleRelativeTransform);
		}
	}
}

void AATAWeapon_Melee::BeginPlay() // AActor
{
	Super::BeginPlay();

	if (Collision)
	{
		Collision->ClearMoveIgnoreActors();
		Collision->IgnoreActorWhenMoving(OwnerCharacter, true);
	}
}
//~ End Initialize
