// G.o.o.P Studios

#include "GAS/DataAssets/WeaponData_Base.h"

UWeaponData_Base::UWeaponData_Base()
{
	DisplayName = FText::FromString(TEXT("Unnamed Weapon"));

	OwnerMeshAttachmentSocket = TEXT("Hand_RSocket");
}
