// Scientific Ways

#include "Gameplay/Combo/ScWComboData.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/Handhelds/ScWHandheldData.h"

UScWComboData::UScWComboData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Combo"));

	DamageMul = 1.0f;
	DamageAdditive = 0.0f;
	DamageImpactImpulseMul = 1.0f;
}

//~ Begin Damage
float UScWComboData::BP_ModifyHandheldDamage_Implementation(const UScWHandheldData* InHandheldData, float InDamage) const
{
	return InDamage * DamageMul + DamageAdditive;
}

TSubclassOf<UScWDamageType> UScWComboData::BP_ModifyHandheldDamageTypeClass_Implementation(const UScWHandheldData* InHandheldData, TSubclassOf<UScWDamageType> InDamageTypeClass) const
{
	return OverrideDamageTypeClass ? OverrideDamageTypeClass : InDamageTypeClass;
}
//~ End Damage

//~ Begin Swing Variants
void UScWComboData::BP_ModifySwingVariants_Implementation(const UScWHandheldData* InHandheldData, const TArray<FScWMeleeSwingVariantData>& InVariants, TArray<FScWMeleeSwingVariantData>& OutVariants) const
{
	OutVariants = OverrideSwingVariants.IsEmpty() ? InVariants : OverrideSwingVariants;
}
//~ End Swing Variants
