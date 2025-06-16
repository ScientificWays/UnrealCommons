// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "Types_Delegates.h"

#include "GASFunctionLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FGameplayTagNumChangedEvent, const FGameplayTag, InTag, int32, InNewNum);

/**
 * 
 */
UCLASS()
class ATA_DREAMCOMETRUE_API UGASFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//~ Begin Health
public:

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static TSubclassOf<UGameplayEffect> GetSetHealthGameplayEffectClass(const UObject* InWCO);

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static TSubclassOf<UGameplayEffect> GetAddHealthGameplayEffectClass(const UObject* InWCO);

//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static TSubclassOf<UGameplayEffect> GetApplyDamageGameplayEffectClassForType(const UObject* InWCO, const UDamageType* InDamageType);

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintAuthorityOnly)
	static float ApplyPointDamage(AActor* InSourceActor, AActor* InTargetActor, float InDamage, const FVector& InHitFromDirection, const FHitResult& InHitResult, AController* InInstigator, TSubclassOf<UDamageType> InDamageTypeClass);

	//UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintAuthorityOnly)
	//static void HandleFirearmBulletHit(UIDASC_Base* InShootingASC, const FHitResult& InHitResult, const class UFirearmDataAsset* InFirearmAsset);

	//UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintAuthorityOnly)
	//static void HandleMeleeSwingHit(UIDASC_Character* InSwingingASC, const FHitResult& InHitResult, const UMeleeDataAsset* InMeleeAsset);
//~ End Damage

//~ Begin GameplayTags
public:

	UFUNCTION(Category = "GameplayTags", BlueprintCallable)
	static void AddLooseGameplayTag(UAbilitySystemComponent* InASC, FGameplayTag InGameplayTag, int32 InCount = 1)
	{ InASC->AddLooseGameplayTag(InGameplayTag, InCount); }

	UFUNCTION(Category = "GameplayTags", BlueprintCallable)
	static void AddLooseGameplayTags(UAbilitySystemComponent* InASC, FGameplayTagContainer InGameplayTags, int32 InCount = 1)
	{ InASC->AddLooseGameplayTags(InGameplayTags, InCount); }

	UFUNCTION(Category = "GameplayTags", BlueprintCallable)
	static void RemoveLooseGameplayTag(UAbilitySystemComponent* InASC, FGameplayTag InGameplayTag, int32 InCount = 1)
	{ InASC->RemoveLooseGameplayTag(InGameplayTag, InCount); }

	UFUNCTION(Category = "GameplayTags", BlueprintCallable)
	static void RemoveLooseGameplayTags(UAbilitySystemComponent* InASC, FGameplayTagContainer InGameplayTags, int32 InCount = 1)
	{ InASC->RemoveLooseGameplayTags(InGameplayTags, InCount); }

	UFUNCTION(Category = "GameplayTags", BlueprintCallable)
	static void SetLooseGameplayTagCount(UAbilitySystemComponent* InASC, FGameplayTag InGameplayTag, int32 InNewCount)
	{ InASC->SetLooseGameplayTagCount(InGameplayTag, InNewCount); }

	UFUNCTION(Category = "GameplayTags", BlueprintCallable)
	static FDelegateHandle_BlueprintWrapper RegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FGameplayTagNumChangedEvent InEvent, FGameplayTag InTag, EGameplayTagEventType::Type InType = EGameplayTagEventType::NewOrRemoved);

	UFUNCTION(Category = "GameplayTags", BlueprintCallable)
	static void UnRegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FDelegateHandle_BlueprintWrapper InHandleWrapper, FGameplayTag InTag, EGameplayTagEventType::Type InType = EGameplayTagEventType::NewOrRemoved);
//~ End GameplayTags

//~ Begin GameplayEffects
public:

	UFUNCTION(Category = "GameplayEffects", BlueprintCallable, BlueprintPure)
	static bool IsValidEffectHandle(UPARAM(Ref) const FActiveGameplayEffectHandle& InHandle)
	{ return InHandle.IsValid(); }

	UFUNCTION(Category = "GameplayEffects", BlueprintCallable)
	static FActiveGameplayEffectHandle& InvalidateEffectHandle(UPARAM(Ref) FActiveGameplayEffectHandle& InHandle)
	{ InHandle.Invalidate(); return InHandle; }

	UFUNCTION(Category = "GameplayEffects", BlueprintCallable, BlueprintPure)
	static bool WasSuccessfullyAppliedEffectHandle(UPARAM(Ref) const FActiveGameplayEffectHandle& InHandle)
	{ return InHandle.WasSuccessfullyApplied(); }
//~ End GameplayEffects
};
