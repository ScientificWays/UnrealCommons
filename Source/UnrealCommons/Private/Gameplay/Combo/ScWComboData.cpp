// Scientific Ways

#include "Gameplay/Combo/ScWComboData.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/Combo/ScWComboMoveData.h"
#include "Gameplay/Handhelds/ScWHandheldData.h"

UScWComboData::UScWComboData()
{
	DisplayName = FText::FromString(TEXT("Unnamed Combo"));

	bUseLastMoveModifiers = true;

	ComboPowerMul = 1.0f;

	DamageMul = 1.0f;
	DamageAdditive = 0.0f;
	DamageImpactImpulseMul = 1.0f;
}

//~ Begin Power
float UScWComboData::BP_GetHandheldPowerMul_Implementation(const UScWHandheldData* InHandheldData) const
{
	auto OutPowerMul = ComboPowerMul;

	if (bUseLastMoveModifiers)
	{
		ensureReturn(!Moves.IsEmpty(), OutPowerMul);
		auto LastMove = Moves.Last();

		OutPowerMul *= LastMove->MovePowerMul;
	}
	return OutPowerMul;
}
//~ End Power

//~ Begin Damage
float UScWComboData::BP_ModifyHandheldDamage_Implementation(const UScWHandheldData* InHandheldData, float InDamage) const
{
	float FinalDamageMul = DamageMul;
	float FinalDamageAdditive = DamageAdditive;

	if (bUseLastMoveModifiers)
	{
		ensureReturn(!Moves.IsEmpty(), InDamage);
		auto LastMove = Moves.Last();

		FinalDamageMul *= LastMove->MoveDamageMul;
		FinalDamageAdditive += LastMove->MoveDamageAdditive;
	}
	return InDamage * FinalDamageMul + FinalDamageAdditive;
}

TSubclassOf<UScWDamageType> UScWComboData::BP_ModifyHandheldDamageTypeClass_Implementation(const UScWHandheldData* InHandheldData, TSubclassOf<UScWDamageType> InDamageTypeClass) const
{
	if (OverrideDamageTypeClass)
	{
		return OverrideDamageTypeClass;
	}
	else if (bUseLastMoveModifiers)
	{
		ensureReturn(!Moves.IsEmpty(), InDamageTypeClass);
		auto LastMove = Moves.Last();

		if (LastMove->OverrideMoveDamageTypeClass)
		{
			return LastMove->OverrideMoveDamageTypeClass;
		}
	}
	return InDamageTypeClass;
}
//~ End Damage

//~ Begin Swing Variants
void UScWComboData::BP_ModifySwingVariants_Implementation(const UScWHandheldData* InHandheldData, const TArray<FScWMeleeSwingVariantData>& InVariants, TArray<FScWMeleeSwingVariantData>& OutVariants) const
{
	OutVariants = OverrideSwingVariants.IsEmpty() ? InVariants : OverrideSwingVariants;
}
//~ End Swing Variants
