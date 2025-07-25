// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "ScWGameplayFunctionLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FGameplayTagNumChangedEvent, const FGameplayTag, InTag, int32, InNewNum);

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] Gameplay Function Library"))
class UNREALCOMMONS_API UScWGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//~ Begin Health
public:

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static TSubclassOf<UGameplayEffect> GetSetHealthGameplayEffectClass(const UObject* InWCO);

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static TSubclassOf<UGameplayEffect> GetAddHealthGameplayEffectClass(const UObject* InWCO);

	UFUNCTION(Category = "Health", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static TSubclassOf<UGameplayEffect> GetDeadStateGameplayEffectClass(const UObject* InWCO);
//~ End Health
	
//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO"))
	static TSubclassOf<UGameplayEffect> GetApplyDamageGameplayEffectClassForType(const UObject* InWCO, const UDamageType* InDamageType);

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintAuthorityOnly)
	static float ApplyPointDamage(AActor* InSourceActor, AActor* InTargetActor, float InDamage, const FVector& InHitFromDirection, const FHitResult& InHitResult, AController* InInstigator, TSubclassOf<UDamageType> InDamageTypeClass);

	UFUNCTION(Category = "Damage", BlueprintCallable, BlueprintPure, meta = (KeyWords = "GetHitResult, GetMinimalInfoHitResult, GetMinimalHitResult, MakeHitResult, MakeMinimalHitResult"))
	static void MakeMinimalInfoDamageImpactHitResult(AActor* InSourceActor, UPrimitiveComponent* InSourceComponent, APawn* InInstigatorPawn, AActor* InDamagedActor, UPrimitiveComponent* InDamagedComponent, FHitResult& OutHitResult);
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

//~ Begin Input
public:

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (AutoCreateRefTerm = "InOptions", KeyWords = "AddInputMappingContextTo"))
	static bool AddEnhancedInputMappingContextTo(APlayerController* InPlayerController, const UInputMappingContext* InMappingContext, int32 InPriority, const FModifyContextOptions& InOptions = FModifyContextOptions());

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (AutoCreateRefTerm = "InOptions", KeyWords = "RemoveInputMappingContextFrom"))
	static bool RemoveEnhancedInputMappingContextFrom(APlayerController* InPlayerController, const UInputMappingContext* InMappingContext, const FModifyContextOptions& InOptions = FModifyContextOptions());
	
	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, BlueprintPure)
	static bool GetActionMappedKeyDisplayName(APlayerController* InPlayerController, const UInputAction* InInputAction, const bool bInLongDisplayName, FText& OutDisplayName);
//~ End Input
	
//~ Begin Visibility
public:

	UFUNCTION(Category = "Visibility", BlueprintCallable, BlueprintPure)
	static bool IsComponentRenderedFor(UPrimitiveComponent* InComponent, AActor* ForActor);
//~ End Visibility
};
