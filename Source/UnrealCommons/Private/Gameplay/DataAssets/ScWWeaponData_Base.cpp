// Scientific Ways

#include "Gameplay/DataAssets/ScWWeaponData_Base.h"

UScWWeaponData_Base::UScWWeaponData_Base()
{
	DisplayName = FText::FromString(TEXT("Unnamed Weapon"));

	OwnerMeshAttachmentSocket = TEXT("Hand_RSocket");
}
