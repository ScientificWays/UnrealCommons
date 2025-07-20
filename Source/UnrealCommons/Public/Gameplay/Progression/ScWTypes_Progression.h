// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_Progression.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScWProgressionTaskEventSignature, class AScWPlayerState*, InPlayerState, const class UScWProgressionTaskData*, InTaskData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FScWProgressionTaskMeterEventSignature, class AScWPlayerState*, InPlayerState, const class UScWProgressionTaskData*, InTaskData, const FName&, InMeterName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FScWProgressionTaskMeterValueEventSignature, class AScWPlayerState*, InPlayerState, const class UScWProgressionTaskData*, InTaskData, const FName&, InMeterName, const int32, InValue);

USTRUCT(BlueprintType)
struct FScWProgressionTask_MeterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (KeyWords = "DisplayName"))
	FText DisplayTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush ImageBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetValue;

	FScWProgressionTask_MeterData();

	static const FScWProgressionTask_MeterData Invalid;
};

USTRUCT(BlueprintType)
struct FScWProgressionTask_ProgressData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> MetersProgressMap;

	static const FScWProgressionTask_ProgressData Invalid;
};
