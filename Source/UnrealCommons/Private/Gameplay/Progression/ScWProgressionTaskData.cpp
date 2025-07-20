// Scientific Ways

#include "Gameplay/Progression/ScWProgressionTaskData.h"

UScWProgressionTaskData::UScWProgressionTaskData()
{
	DisplayTitle = FText::FromString(TEXT("Progression Task's title."));
	Description = FText::FromString(TEXT("Progression Task's description."));

	ImageBrush.SetImageSize({ 96.0f, 96.0f });
}

//~ Begin Meters
bool UScWProgressionTaskData::HasMeter(const FName& InMeterName) const
{
	return MetersMap.Contains(InMeterName);
}

const FScWProgressionTask_MeterData& UScWProgressionTaskData::GetMeterData(const FName& InMeterName) const
{
	if (const FScWProgressionTask_MeterData* OutData = MetersMap.Find(InMeterName))
	{
		return *OutData;
	}
	return FScWProgressionTask_MeterData::Invalid;
}

int32 UScWProgressionTaskData::GetMeterTargetValue(const FName& InMeterName) const
{
	if (const FScWProgressionTask_MeterData* SampleData = MetersMap.Find(InMeterName))
	{
		return SampleData->TargetValue;
	}
	return INDEX_NONE;
}
//~ End Meters
