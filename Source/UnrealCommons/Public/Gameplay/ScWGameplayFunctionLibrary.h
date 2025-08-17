// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "Gameplay/ScWTypes_Gameplay.h"

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

//~ Begin Gameplay Tags
public:

	UFUNCTION(Category = "Gameplay Tags", BlueprintCallable)
	static void AddLooseGameplayTag(UAbilitySystemComponent* InASC, FGameplayTag InGameplayTag, int32 InCount = 1)
	{ InASC->AddLooseGameplayTag(InGameplayTag, InCount); }

	UFUNCTION(Category = "Gameplay Tags", BlueprintCallable)
	static void AddLooseGameplayTags(UAbilitySystemComponent* InASC, FGameplayTagContainer InGameplayTags, int32 InCount = 1)
	{ InASC->AddLooseGameplayTags(InGameplayTags, InCount); }

	UFUNCTION(Category = "Gameplay Tags", BlueprintCallable)
	static void RemoveLooseGameplayTag(UAbilitySystemComponent* InASC, FGameplayTag InGameplayTag, int32 InCount = 1)
	{ InASC->RemoveLooseGameplayTag(InGameplayTag, InCount); }

	UFUNCTION(Category = "Gameplay Tags", BlueprintCallable)
	static void RemoveLooseGameplayTags(UAbilitySystemComponent* InASC, FGameplayTagContainer InGameplayTags, int32 InCount = 1)
	{ InASC->RemoveLooseGameplayTags(InGameplayTags, InCount); }

	UFUNCTION(Category = "Gameplay Tags", BlueprintCallable)
	static void SetLooseGameplayTagCount(UAbilitySystemComponent* InASC, FGameplayTag InGameplayTag, int32 InNewCount)
	{ InASC->SetLooseGameplayTagCount(InGameplayTag, InNewCount); }

	UFUNCTION(Category = "Gameplay Tags", BlueprintCallable)
	static FDelegateHandle_BlueprintWrapper RegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FGameplayTagNumChangedEvent InEvent, FGameplayTag InTag, EGameplayTagEventType::Type InType = EGameplayTagEventType::NewOrRemoved);

	UFUNCTION(Category = "Gameplay Tags", BlueprintCallable)
	static void UnRegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FDelegateHandle_BlueprintWrapper InHandleWrapper, FGameplayTag InTag, EGameplayTagEventType::Type InType = EGameplayTagEventType::NewOrRemoved);
//~ End Gameplay Tags

//~ Begin Gameplay Effects
public:

	UFUNCTION(Category = "Gameplay Effects", BlueprintCallable, BlueprintPure)
	static bool IsValidEffectHandle(UPARAM(Ref) const FActiveGameplayEffectHandle& InHandle) { return InHandle.IsValid(); }

	UFUNCTION(Category = "Gameplay Effects", BlueprintCallable)
	static FActiveGameplayEffectHandle& InvalidateEffectHandle(UPARAM(Ref) FActiveGameplayEffectHandle& InHandle) { InHandle.Invalidate(); return InHandle; }

	UFUNCTION(Category = "Gameplay Effects", BlueprintCallable, BlueprintPure)
	static bool WasSuccessfullyAppliedEffectHandle(UPARAM(Ref) const FActiveGameplayEffectHandle& InHandle) { return InHandle.WasSuccessfullyApplied(); }
//~ End Gameplay Effects

//~ Begin Gameplay Abilities
public:
	
	UFUNCTION(Category = "Gameplay Abilities", BlueprintCallable, BlueprintPure)
	static bool IsValidAbilitySpecHandle(UPARAM(Ref) const FGameplayAbilitySpecHandle& InHandle) { return InHandle.IsValid(); }

	UFUNCTION(Category = "Gameplay Abilities", BlueprintCallable)
	static FGameplayAbilitySpecHandle& InvalidateAbilitySpecHandle(UPARAM(Ref) FGameplayAbilitySpecHandle& InHandle) { InHandle = FGameplayAbilitySpecHandle(); return InHandle; }

//~ End Gameplay Abilities

//~ Begin Input
public:

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (AutoCreateRefTerm = "InOptions", KeyWords = "AddInputMappingContextTo"))
	static bool AddEnhancedInputMappingContextTo(APlayerController* InPlayerController, const UInputMappingContext* InMappingContext, int32 InPriority, const FModifyContextOptions& InOptions = FModifyContextOptions());

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, meta = (AutoCreateRefTerm = "InOptions", KeyWords = "RemoveInputMappingContextFrom"))
	static bool RemoveEnhancedInputMappingContextFrom(APlayerController* InPlayerController, const UInputMappingContext* InMappingContext, const FModifyContextOptions& InOptions = FModifyContextOptions());
	
	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintCosmetic, BlueprintPure)
	static bool GetActionMappedKeyDisplayName(APlayerController* InPlayerController, const UInputAction* InInputAction, const bool bInLongDisplayName, FText& OutDisplayName);

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintPure, meta = (DisplayName = "Enum to AbilityInputID"))
	static int32 EnumToAbilityInputID(EScWAbilityInputID InEnum) { return static_cast<int32>(InEnum); }

	UFUNCTION(Category = "Input", BlueprintCallable, BlueprintPure, meta = (DisplayName = "AbilityInputID to Enum"))
	static EScWAbilityInputID AbilityInputIDToEnum(int32 InInputID) { return static_cast<EScWAbilityInputID>(InInputID); }
//~ End Input
	
//~ Begin Visibility
public:

	UFUNCTION(Category = "Visibility", BlueprintCallable, BlueprintPure)
	static bool IsComponentRenderedFor(UPrimitiveComponent* InComponent, AActor* ForActor);
//~ End Visibility
	
//~ Begin Materials
public:

	UFUNCTION(Category = "Materials", BlueprintCallable)
	static void ApplyOverrideMaterialsToMeshComponent(UMeshComponent* InMeshComponent, const TMap<int32, UMaterialInterface*>& InOverrideMaterials);
//~ End Materials
	
//~ Begin Interact
public:

	UFUNCTION(Category = "Interact", BlueprintCallable, meta = (WorldContext = "InWCO", DeterminesOutputType = "InActorArray"))
	static TArray<AActor*> FilterActorsByCanInteract(const UObject* InWCO, class UScWInteractComponent* InSourceInteractComponent, const TArray<AActor*>& InActorArray, const bool bReverseCondition, const bool bInReturnAllInsteadOfNothing = false, const bool bInReturnFirstValidActor = false);

	UFUNCTION(Category = "Interact", BlueprintCallable, BlueprintPure, meta = (WorldContext = "InWCO", AutoCreateRefTerm = "InTraceDebugData, InActorFilterArray"))
	static class UScWInteractComponent* FindInteractTargetInLocation(const UObject* InWCO, const FVector& InLocation, const float InRadius, ETraceTypeQuery InTraceTypeQuery, const struct FScWTraceDebugData& InTraceDebugData, const TArray<AActor*>& InActorFilterArray);
//~ End Interact

//~ Begin Teams
public:

	UFUNCTION(Category = "Teams", BlueprintCallable, meta = (KeyWords = "GetTeamOfActor"))
	static FGenericTeamId GetActorTeamId(const AActor* InActor, const bool bInChecked = false);

	UFUNCTION(Category = "Teams", BlueprintCallable, meta = (KeyWords = "GetTeamOfActor"))
	static const FName& GetActorTeamName(const AActor* InActor, const bool bInChecked = false);

	UFUNCTION(Category = "Teams", BlueprintCallable, meta = (WorldContext = "InWCO", KeyWords = "GetActorsOfTeam, GetCharactersOfTeam, GetAllCharactersOfTeam", AutoCreateRefTerm = "InTeamName", DeterminesOutputType = "InFilterActorClass"))
	static TArray<AActor*> GetAllActorsOfTeam(const UObject* InWCO, const FName& InTeamName, TSubclassOf<AActor> InFilterActorClass = nullptr);

	UFUNCTION(Category = "Teams", BlueprintCallable, meta = (WorldContext = "InWCO", KeyWords = "GetActorsOfTeamSet, GetCharactersOfTeamSet, GetAllCharactersOfTeamSet", AutoCreateRefTerm = "InTeamName", DeterminesOutputType = "InFilterActorClass"))
	static TArray<AActor*> GetAllActorsOfAnyTeam(const UObject* InWCO, const TSet<FName>& InTeamNameSet, TSubclassOf<AActor> InFilterActorClass = nullptr);
//~ End Teams

//~ Begin Transforms
public:

	UFUNCTION(Category = "Transforms", BlueprintCallable, BlueprintPure, meta = (KeyWords = "Location, Position, Middle", AutoCreateRefTerm = "InVector, InGridSize"))
	static FVector VectorSnappedToGridCenter(const FVector& InVector, const FVector& InGridSize);
//~ End Transforms
};
