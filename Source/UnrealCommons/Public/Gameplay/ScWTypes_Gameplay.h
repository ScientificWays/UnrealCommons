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

/*USTRUCT(BlueprintType)
struct FScWGameplayAbilityActorInfo : public FGameplayAbilityActorInfo
{
	GENERATED_BODY()

	virtual void InitFromActor(AActor* InOwnerActor, AActor* InAvatarActor, UAbilitySystemComponent* InAbilitySystemComponent) override;
};*/
