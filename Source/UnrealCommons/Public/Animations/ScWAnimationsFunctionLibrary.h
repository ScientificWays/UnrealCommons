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

public:
	static const FName MontageDefaultGroupName;
	
//~ Begin Montages
public:

	UFUNCTION(Category = "Montages", BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "InFallbackValue", KeyWords = "GetDuration"))
	static float GetMontagePlayLength(const UAnimMontage* InMontage, float InTimeMul = 1.0f, float InFallbackValue = 0.0f);

	UFUNCTION(Category = "Montages", BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "InFallbackValue", KeyWords = "GetDuration, GetSectionLength"))
	static float GetMontageSectionLengthByIndex(const UAnimMontage* InMontage, int32 InIndex, float InTimeMul = 1.0f, float InFallbackValue = 0.0f);

	UFUNCTION(Category = "Montages", BlueprintCallable, BlueprintPure, meta = (AutoCreateRefTerm = "InName", AdvancedDisplay = "InFallbackValue", KeyWords = "GetDuration, GetSectionLength"))
	static float GetMontageSectionLengthByName(const UAnimMontage* InMontage, const FName& InName, float InTimeMul = 1.0f, float InFallbackValue = 0.0f);

	UFUNCTION(Category = "Montages | Character Data", BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "InFallbackValue", KeyWords = "GetDuration"))
	static float GetMontagePlayLengthFromData(const struct FScWCharacterMontageData& InCharacterMontageData, float InFallbackValue = 0.0f);

	UFUNCTION(Category = "Montages | Character Data", BlueprintCallable, BlueprintPure, meta = (AdvancedDisplay = "InFallbackValue", KeyWords = "GetDuration, GetSectionLength"))
	static float GetMontageSectionLengthByIndexFromData(const struct FScWCharacterMontageData& InCharacterMontageData, int32 InIndex, float InFallbackValue = 0.0f);

	UFUNCTION(Category = "Montages | Character Data", BlueprintCallable, BlueprintPure, meta = (AutoCreateRefTerm = "InName", AdvancedDisplay = "InFallbackValue", KeyWords = "GetDuration, GetSectionLength"))
	static float GetMontageSectionLengthByNameFromData(const struct FScWCharacterMontageData& InCharacterMontageData, const FName& InName, float InFallbackValue = 0.0f);

	UFUNCTION(Category = "Montages | Character Data", BlueprintCallable, meta = (ReturnDisplayName = "Out Max Duration", KeyWords = "PlayMontageData, PlayCharacterMontageData"))
	static float PlayCharacterMontagesFromData(class AScWCharacter* InCharacter, const struct FScWCharacterMontageData& InCharacterMontageData, const bool bInStopAllMontages = true);
//~ End Montages

//~ Begin Static Data
public:
	
	UFUNCTION(Category = "Static Data", BlueprintCallable, BlueprintCosmetic)
	static void SetCharacterDefaultStaticAnimationData(class AScWCharacter* InCharacter, const struct FScWFirstPersonStaticAnimationData& InFirstPersonStaticAnimationData, const struct FScWThirdPersonStaticAnimationData& InThirdPersonStaticAnimationData, const bool bInApplyDefaultStaticAnimationData);
	
	UFUNCTION(Category = "Static Data", BlueprintCallable, BlueprintCosmetic)
	static void ToggleCharacterHolsterStaticAnimationData(class AScWCharacter* InCharacter, const bool bInUseHolster, const float InBlendTime, const bool bInFirstPerson = true, const bool bInThirdPerson = true);

	UFUNCTION(Category = "Static Data", BlueprintCallable, BlueprintCosmetic)
	static void ToggleCharacterAlternativeBasePose(class AScWCharacter* InCharacter, const bool bInUseAlternative, const float InBlendTime, const bool bInFirstPerson = true, const bool bInThirdPerson = true);

	UFUNCTION(Category = "Static Data", BlueprintCallable, BlueprintCosmetic)
	static void ToggleCharacterEnableIdleAnimation(class AScWCharacter* InCharacter, const bool bInEnable, const bool bInFirstPerson = true, const bool bInThirdPerson = true);
//~ End Static Data
};
