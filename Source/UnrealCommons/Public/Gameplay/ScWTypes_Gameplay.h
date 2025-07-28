// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_Gameplay.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "[ScW] Ability Input ID"))
enum class EScWAbilityInputID : uint8
{
	UsePrimary,
	UseSecondary,
	Jump,
	Crouch,
	Interact,
	Reload,
	Sprint,
	Walk,
	Shove,
	Drop,
	Flashlight,
	Special
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Gameplay Ability Info"))
struct FScWGameplayGiveAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAssignInputID = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bAssignInputID"))
	EScWAbilityInputID InputID = EScWAbilityInputID::UsePrimary;
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Interact Data"))
struct FScWInteractData
{
	GENERATED_BODY()

	UPROPERTY(Category = "Interact", Transient, BlueprintReadOnly)
	TObjectPtr<class UScWInteractComponent> Source = nullptr;

	UPROPERTY(Category = "Interact", Transient, BlueprintReadOnly)
	TObjectPtr<class UScWInteractComponent> Target = nullptr;

	UPROPERTY(Category = "Interact", Transient, BlueprintReadOnly)
	float StartTime = 0.0f;

	UPROPERTY(Category = "Interact", Transient, BlueprintReadOnly)
	float FinishTime = 0.0f;

	static const FScWInteractData Invalid;
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Trace Debug Data"))
struct FScWTraceDebugData
{
	GENERATED_BODY()

	UPROPERTY(Category = Trace, BlueprintReadWrite)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(Category = Trace, BlueprintReadWrite)
	float DrawTime = 5.0f;

	UPROPERTY(Category = Trace, BlueprintReadWrite)
	FLinearColor TraceColor = FLinearColor::Green;

	UPROPERTY(Category = Trace, BlueprintReadWrite)
	FLinearColor TraceHitColor = FLinearColor::Red;

	static const FScWTraceDebugData None;
};
