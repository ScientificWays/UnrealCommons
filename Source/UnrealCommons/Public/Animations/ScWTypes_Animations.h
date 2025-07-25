// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_Animations.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Handheld StaticAnimationData"))
struct FScWHandheldStaticAnimationData
{
	GENERATED_BODY()

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimSequence> BasePose = nullptr;

	UPROPERTY(Category = "Static", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimSequence> AlternativeBasePose = nullptr;

	bool IsValid() const { return BasePose && AlternativeBasePose; }
	static const FScWHandheldStaticAnimationData None;
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] FirstPerson StaticAnimationData"))
struct FScWFirstPersonStaticAnimationData
{
	GENERATED_BODY()

	UPROPERTY(Category = "Base", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimSequence> BasePose = nullptr;
	
	UPROPERTY(Category = "Base", BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0.0"))
	bool bBasePosePitchIK = true;
	
	UPROPERTY(Category = "Base", BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bBasePosePitchIK", ClampMin = "0.0"))
	float BasePosePitchIKScale = 0.05f;

	UPROPERTY(Category = "Base", BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0.0"))
	float BasePoseWalkBlendAlphaScale = 0.5f;
	
	UPROPERTY(Category = "Alternative", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimSequence> AlternativeBasePose = nullptr;
	
	UPROPERTY(Category = "Alternative", BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0.0"))
	bool bAlternativeBasePosePitchIK = false;
	
	UPROPERTY(Category = "Alternative", BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bAlternativeBasePosePitchIK", ClampMin = "0.0"))
	float AlternativeBasePosePitchIKScale = 0.05f;

	UPROPERTY(Category = "Alternative", BlueprintReadOnly, EditAnywhere, meta = (ClampMin = "0.0"))
	float AlternativeBasePoseWalkBlendAlphaScale = 0.05f;

	bool IsValid() const { return BasePose && AlternativeBasePose; }
	static const FScWFirstPersonStaticAnimationData None;
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] ThirdPerson StaticAnimationData"))
struct FScWThirdPersonStaticAnimationData
{
	GENERATED_BODY()
		
	UPROPERTY(Category = "UpperBody", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAnimSequence> UpperBodyIdle = nullptr;

	UPROPERTY(Category = "UpperBody", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAimOffsetBlendSpace> UpperBodyAimOffset = nullptr;

	UPROPERTY(Category = "UpperBody", BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UAimOffsetBlendSpace> UpperBodyAlternativeAimOffset = nullptr;

	bool IsValid() const { return UpperBodyIdle && UpperBodyAimOffset && UpperBodyAlternativeAimOffset; }
	static const FScWThirdPersonStaticAnimationData None;
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Character MontageData"))
struct FScWCharacterMontageData
{
	GENERATED_BODY()

	UPROPERTY(Category = "Montage", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FP: Character Montage"))
	TObjectPtr<UAnimMontage> FirstPersonMontage = nullptr;

	UPROPERTY(Category = "Montage", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FP: Handheld Montage"))
	TObjectPtr<UAnimMontage> FirstPersonActiveHandheldMontage = nullptr;

	UPROPERTY(Category = "Montage", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TP: Character Montage"))
	TObjectPtr<UAnimMontage> ThirdPersonMontage = nullptr;
	
	UPROPERTY(Category = "Montage", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TP: Handheld Montage"))
	TObjectPtr<UAnimMontage> ThirdPersonActiveHandheldMontage = nullptr;

	UPROPERTY(Category = "Montage", EditAnywhere, BlueprintReadWrite)
	int32 TimingMontageIndex = 0;

	UPROPERTY(Category = "Shake", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraShakeBase> FirstPersonCameraShake = nullptr;

	UPROPERTY(Category = "Time", EditAnywhere, BlueprintReadWrite)
	float TimeMul = 1.0f;

	UAnimMontage* GetRelevantTimingMontage() const;
};
