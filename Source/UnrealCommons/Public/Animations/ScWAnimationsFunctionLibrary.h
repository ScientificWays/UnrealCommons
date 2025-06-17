// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAnimationsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Animations Function Library"))
class UNREALCOMMONS_API UScWAnimationsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//~ Begin Montages
public:

	UFUNCTION(Category = "Montages", BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "InOptionalFallbackValue", KeyWords = "GetDuration"))
	static float GetMontagePlayLength(const UAnimMontage* InMontage, float InTimeMul = 1.0f, float InOptionalFallbackValue = -1.0f);

	UFUNCTION(Category = "Montages", BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "InOptionalFallbackValue", KeyWords = "GetDuration, GetSectionLength"))
	static float GetMontageSectionLengthByIndex(const UAnimMontage* InMontage, int32 InIndex, float InTimeMul = 1.0f, float InOptionalFallbackValue = -1.0f);

	UFUNCTION(Category = "Montages", BlueprintCallable, BlueprintPure, meta = (AutoCreateRefTerm = "InName", AdvancedDisplay = "InOptionalFallbackValue", KeyWords = "GetDuration, GetSectionLength"))
	static float GetMontageSectionLengthByName(const UAnimMontage* InMontage, const FName& InName, float InTimeMul = 1.0f, float InOptionalFallbackValue = -1.0f);
//~ End Montages
};
