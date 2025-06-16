// Scientific Ways

#include "Gameplay/DataAssets/ScWComboData.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/DataAssets/ScWWeaponData_Base.h"

UScWComboData::UScWComboData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Combo"));

	DamageMul = 1.0f;
	DamageAdditive = 0.0f;
}

//~ Begin Damage
float UScWComboData::K2_ModifyWeaponDamage_Implementation(const UScWWeaponData_Base* InWeaponData, float InDamage) const
{
	return InDamage * DamageMul + DamageAdditive;
}

TSubclassOf<UScWDamageType> UScWComboData::K2_ModifyWeaponDamageTypeClass_Implementation(const UScWWeaponData_Base* InWeaponData, TSubclassOf<UScWDamageType> InDamageTypeClass) const
{
	return OverrideDamageTypeClass ? OverrideDamageTypeClass : InDamageTypeClass;
}
