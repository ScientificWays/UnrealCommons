// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "Gameplay/ScWASC_InitInterface.h"

#include "ScWCharacterCapsule.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "[ScW] Character Capsule (Component)"))
class UNREALCOMMONS_API UScWCharacterCapsule : public UCapsuleComponent, public IScWASC_InitInterface, public IScWCharacterData_InitInterface
{
	GENERATED_BODY()
	
public:	
	
	UScWCharacterCapsule();
	
//~ Begin Initialize
public:
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) override; // IScWASC_InitInterface
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
protected:
	virtual void BeginPlay() override; // UActorComponent
//~ End Initialize

//~ Begin Profiles
public:

	UPROPERTY(Category = "Profiles", EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FName> TagCollisionProfileNameMap;

	UPROPERTY()
	FName DefaultCollisionProfileName;
//~ End Profiles

//~ Begin Tags
protected:
	void OnCollisionTagNumChanged(const FGameplayTag InCallbackTag, int32 InNewCount);
	void UpdateCollisionProfileFromTags();
//~ End Tags

//~ Begin Damage
protected:

	UFUNCTION()
	void HandleDamageApplied(float InDamage, const struct FReceivedDamageData& InData);

	UFUNCTION()
	void HandleDied();
//~ End Damage
};
