// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Progression/ScWTypes_Progression.h"

#include "ScWProgressionTaskData.generated.h"

/**
 *
 */
UCLASS(const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Progression Task Data"))
class UNREALCOMMONS_API UScWProgressionTaskData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	

	UScWProgressionTaskData();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (KeyWords = "DisplayName"))
	FText DisplayTitle;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FSlateBrush ImageBrush;
//~ End UI

//~ Begin Meters
public:

	UFUNCTION(Category = "Voxel Tree", BlueprintCallable)
	bool HasMeter(const FName& InMeterName) const;

	UFUNCTION(Category = "Voxel Tree", BlueprintCallable)
	const FScWProgressionTask_MeterData& GetMeterData(const FName& InMeterName) const;

	UFUNCTION(Category = "Voxel Tree", BlueprintCallable)
	int32 GetMeterTargetValue(const FName& InMeterName) const;

	UPROPERTY(Category = "Meters", EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FScWProgressionTask_MeterData> MetersMap;
//~ End Meters
};
