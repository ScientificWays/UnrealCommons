// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "AnimationsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UAnimationsFunctionLibrary : public UBlueprintFunctionLibrary
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
