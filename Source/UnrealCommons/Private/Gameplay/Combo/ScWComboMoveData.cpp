// Scientific Ways

#include "Gameplay/Combo/ScWComboMoveData.h"

UScWComboMoveData::UScWComboMoveData()
{
	DisplayName = FText::FromString(TEXT("Unnamed ComboMove"));

	MovePowerMul = 1.0f;

	MoveDamageMul = 1.0f;
	MoveDamageAdditive = 0.0f;
	MoveDamageImpactImpulseMul = 1.0f;
}
