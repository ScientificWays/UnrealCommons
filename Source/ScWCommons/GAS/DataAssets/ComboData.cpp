// G.o.o.P Studios

#include "GAS/DataAssets/ComboData.h"

#include "GAS/ATADT_Base.h"
#include "GAS/DataAssets/WeaponData_Base.h"

UComboData::UComboData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Combo"));

	DamageMul = 1.0f;
	DamageAdditive = 0.0f;
}

//~ Begin Damage
float UComboData::K2_ModifyWeaponDamage_Implementation(const UWeaponData_Base* InWeaponData, float InDamage) const
{
	return InDamage * DamageMul + DamageAdditive;
}

TSubclassOf<UATADT_Base> UComboData::K2_ModifyWeaponDamageTypeClass_Implementation(const UWeaponData_Base* InWeaponData, TSubclassOf<UATADT_Base> InDamageTypeClass) const
{
	return OverrideDamageTypeClass ? OverrideDamageTypeClass : InDamageTypeClass;
}
