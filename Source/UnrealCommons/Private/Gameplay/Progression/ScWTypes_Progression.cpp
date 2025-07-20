// Scientific Ways

#include "Gameplay/Progression/ScWTypes_Progression.h"

FScWProgressionTask_MeterData::FScWProgressionTask_MeterData()
{
	DisplayTitle = FText::FromString(TEXT("Progression Task Meter's title."));
	ImageBrush.SetImageSize({ 32.0f, 32.0f });
	TargetValue = 100;
}

const FScWProgressionTask_MeterData FScWProgressionTask_MeterData::Invalid = FScWProgressionTask_MeterData();
const FScWProgressionTask_ProgressData FScWProgressionTask_ProgressData::Invalid = FScWProgressionTask_ProgressData();
