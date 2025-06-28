// Scientific Ways

#include "Gameplay/DataAssets/ScWWeaponData_Melee.h"

UScWWeaponData_Melee::UScWWeaponData_Melee()
{
	DisplayName = FText::FromString(TEXT("Unnamed Melee Weapon"));

	CapsuleRadiusHeight = FVector2D(14.0f, 45.0f);
	CapsuleRelativeTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, -20.0f), FVector::OneVector);

	SwingBaseDamage = 10.0f;
	PostSwingComboTimeWindow = 0.4f;
	SwingAIMaxRange = 128.0f;
	SwingAIMaxRange_BlackboardKeyName = TEXT("MeleeRangeMax");

	bIsUsingPatterns = false;
	PatternsTriggerTime = 1.0f;
}

//~ Begin Patterns
float UScWWeaponData_Melee::GetNextPatternDelayTime(int32 InNextPatternIndex) const
{
	return (Patterns.Num() < 2) ? (0.0f) : (PatternsTriggerTime / (float)Patterns.Num());
}
//~ End Patterns
