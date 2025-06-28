// Scientific Ways

#include "Gameplay/DataAssets/ScWWeaponData_Base.h"

UScWWeaponData_Base::UScWWeaponData_Base()
{
	DisplayName = FText::FromString(TEXT("Unnamed Weapon"));

	bCanDrop = false;
	bDropOnDeath = false;

	OwnerMeshAttachmentSocket = TEXT("Hand_RSocket");
}
