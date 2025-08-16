// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_CommonDelegates.h"

#include "Gameplay/ScWTypes_Gameplay.h"

#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "ScWASC_Base.generated.h"

UENUM(BlueprintType)
enum class EComboState : uint8
{
	Reset,
	ReadyForMove,
	NotYetReadyForMove,
	Failed,
};

USTRUCT(BlueprintType)
struct FReceivedDamageData
{
	GENERATED_BODY()

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	FHitResult HitResult = FHitResult();

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	TObjectPtr<const class UScWDamageType> DamageType = nullptr;

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> Source = nullptr;

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AController> Instigator = nullptr;

	FReceivedDamageData(const FHitResult& InHitResult = FHitResult(), const class UScWDamageType* InDamageType = nullptr, AActor* InSource = nullptr, AController* InInstigator = nullptr)
		: HitResult(InHitResult), DamageType(InDamageType), Source(InSource), Instigator(InInstigator) {}

	class UScWASC_Base* TryGetAttackerBaseASC() const;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageEventSignature, float, InDamage, const FReceivedDamageData&, InData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAccumulatedDamageResolveEventSignature, float, InDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComboStateChangedSignature, EComboState, InNewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputEventSignature, int32, InInputID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComboMoveEventSignature, const class UScWComboMoveData*, InComboMoveData);

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "[ScW] ASC Base", BlueprintSpawnableComponent))
class UNREALCOMMONS_API UScWASC_Base : public UAbilitySystemComponent, public IScWCharacterData_InitInterface
{
	GENERATED_BODY()
	
public:

	UScWASC_Base();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure)
	static UScWASC_Base* TryGetBaseScWASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface = true);
//~ End Statics
	
//~ Begin Initialize
protected:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override; // UAbilitySystemComponent
	virtual void DestroyActiveState() override; // UAbilitySystemComponent
	virtual void OnRegister() override; // UActorComponent
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
	virtual void BeginPlay() override; // UActorComponent
//~ End Initialize

//~ Begin Attributes
public:
	template<class T>
	const T* GetAttributeSet() { return Cast<const T>(BaseAS); }

	UFUNCTION(Category = "Attributes", BlueprintCallable, meta = (KeyWords = "CurrentHealth"))
	float GetHealth() const;

	UFUNCTION(Category = "Attributes", BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(Category = "Attributes", BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(Category = "Attributes", BlueprintCallable)
	void SetHealth(float InHealth);

	UFUNCTION(Category = "Attributes", BlueprintCallable)
	void AddHealth(float InHealth);

	UPROPERTY(Category = "Attributes", EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldDieOnZeroHealth;

	UPROPERTY(Category = "Attributes", BlueprintAssignable)
	FAttributeChangedSignature OnHealthChanged;

	UPROPERTY(Category = "Attributes", BlueprintAssignable)
	FAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(Category = "Attributes", BlueprintAssignable)
	FDefaultEventSignature OnDied;

protected:

	UPROPERTY(Category = "Attributes", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWAS_Base> AttributeSetClass;

	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& InData);
	virtual void OnMaxHealthAttributeChanged(const FOnAttributeChangeData& InData);

	virtual void OnZeroHealth();
	virtual void HandleDied();

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;

private:

	UPROPERTY()
	TObjectPtr<const UScWAS_Base> BaseAS;
//~ End Attributes
	
//~ Begin Abilities
public:

	UFUNCTION(Category = "Abilities", BlueprintCallable)
	void GiveSpawnAbilities();

	UFUNCTION(Category = "Abilities", BlueprintCallable)
	void GiveAbilitiesFromGiveData(TArray<FScWGameplayGiveAbilityData> InAbilitiesGiveData, TArray<FGameplayAbilitySpecHandle>& OutHandleArray);

	UFUNCTION(Category = "Abilities", BlueprintCallable)
	void ClearAbilities(UPARAM(Ref) TArray<FGameplayAbilitySpecHandle>& InHandleArray, const bool bInResetArray);

	UFUNCTION(Category = "Abilities", BlueprintCallable)
	void GetAbilitiesByInput(EScWAbilityInputID InInputID, TArray<UGameplayAbility*>& OutAbilityArray) const;

	UFUNCTION(Category = "Abilities", BlueprintCallable, meta = (KeyWords = "GiveInputAbilities"))
	void SetInputAbilities(const EScWAbilityInputID InInputID, const TArray<TSubclassOf<UGameplayAbility>>& InClassArray);

	UFUNCTION(Category = "Abilities", BlueprintCallable, meta = (KeyWords = "GiveInputAbility"))
	void SetInputAbility(const EScWAbilityInputID InInputID, TSubclassOf<UGameplayAbility>& InClass) { SetInputAbilities(InInputID, { InClass }); }

	UFUNCTION(Category = "Abilities", BlueprintCallable)
	FGameplayAbilitySpecHandle AddInputAbility(const EScWAbilityInputID InInputID, const TSubclassOf<UGameplayAbility> InClass);

	UPROPERTY(Category = "Abilities", BlueprintReadWrite, EditAnywhere)
	TArray<FScWGameplayGiveAbilityData> SpawnAbilitiesGiveData;

protected:

	UPROPERTY(Category = "Abilities", BlueprintReadOnly)
	TArray<FGameplayAbilitySpecHandle> SpawnAbilitiesSpecsHandles;

	template<class T>
	void AppendAbilityInstancesOrCDO(const FGameplayAbilitySpec& InSpec, TArray<T*>& OutArray) const;
//~ End Abilities

//~ Begin Effects
public:

	UFUNCTION(Category = "Effects", BlueprintCallable)
	FActiveGameplayEffectHandle TryApplyGameplayEffectByClass(TSubclassOf<UGameplayEffect> InClass, float InDuration = -1.0f, float InMagnitude = 0.0f, float InLevel = 1.0f);

	UFUNCTION(Category = "Effects", BlueprintCallable)
	int32 TryRemoveGameplayEffectByClass(TSubclassOf<UGameplayEffect> InClass, int32 InStacksToRemove = -1);

	UFUNCTION(Category = "Effects", BlueprintCallable)
	void ApplySpawnEffect();

	//UPROPERTY(Category = "Effects", BlueprintReadWrite, EditAnywhere)
	UPROPERTY(Transient)
	TSubclassOf<UGameplayEffect> SpawnEffectClass;
//~ End Effects

//~ Begin Tags
protected:
	virtual void OnStunnedTagNumChanged(const FGameplayTag InCallbackTag, int32 InNewCount);
//~ End Tags

//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintCallable)
	float GetLastAppliedDamage() const { return LastAppliedDamage; }

	UFUNCTION(Category = "Damage", BlueprintCallable)
	const FReceivedDamageData& GetLastAppliedDamageData() const { return LastAppliedDamageData; }

	UPROPERTY(Category = "Damage", BlueprintAssignable)
	FDamageEventSignature OnDamageIgnored;

	UPROPERTY(Category = "Damage", BlueprintAssignable)
	FDamageEventSignature OnDamageBlocked;

	UPROPERTY(Category = "Damage", BlueprintAssignable)
	FDamageEventSignature OnDamageEvaded;

	UPROPERTY(Category = "Damage", BlueprintAssignable)
	FDamageEventSignature OnDamageApplied;

	UPROPERTY(Category = "Damage | Accumulated | Applied", BlueprintReadOnly)
	bool bEnableAccumulatedAppliedDamage;

	UFUNCTION(Category = "Damage | Accumulated | Applied", BlueprintCallable)
	void AccumulateAppliedDamage(float InDamage, bool bInAutoResolveNextTick = true);

	UPROPERTY(Category = "Damage | Accumulated | Applied", BlueprintReadOnly)
	float AccumulatedAppliedDamage;

	UFUNCTION(Category = "Damage | Accumulated | Applied", BlueprintCallable)
	void RequestResolveAccumulatedAppliedDamageNextTick();
	void ResolveAccumulatedAppliedDamageFromNextTickTimer();
	FTimerHandle AccumulatedAppliedDamageTimerHandle;

	UFUNCTION(Category = "Damage | Accumulated | Applied", BlueprintCallable)
	void ResolveAccumulatedAppliedDamage();

	UPROPERTY(Category = "Damage | Accumulated | Applied", BlueprintAssignable)
	FAccumulatedDamageResolveEventSignature OnAccumulatedAppliedDamageResolved;

	UPROPERTY(Category = "Damage | Accumulated | Applied", BlueprintReadWrite, EditAnywhere)
	float AccumulatedAppliedDamageResetTime;

	UFUNCTION(Category = "Damage | Accumulated | Applied", BlueprintCallable)
	void ResetAccumulatedAppliedDamage();
	FTimerHandle AccumulatedAppliedDamageResetTimerHandle;

protected:

	UFUNCTION()
	void OnAvatarTakePointDamage(AActor* InDamagedActor, float InDamage, AController* InInstigator, FVector InHitLocation, UPrimitiveComponent* InHitComponent, FName InBoneName, FVector InHitDirection, const UDamageType* InDamageType, AActor* InDamageCauser);

	UFUNCTION()
	void OnAvatarTakeRadialDamage(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, FVector InOrigin, const FHitResult& InHitResult, AController* InInstigator, AActor* InDamageCauser);

	bool HandleTryReceiveDamage(float InDamage, const FReceivedDamageData& InData);

	bool ShouldIgnoreAnyAttackFrom(AController* InInstigator) const;
	bool TryIgnoreDamage(float& InOutAdjustedDamage, const FReceivedDamageData& InData);
	bool TryBlockDamage(float& InOutAdjustedDamage, const FReceivedDamageData& InData);
	bool TryEvadeDamage(float& InOutAdjustedDamage, const FReceivedDamageData& InData);
	bool TryApplyDamage(float InDamage, const FReceivedDamageData& InData);

	UPROPERTY(Category = "Damage", BlueprintReadOnly)
	float LastAppliedDamage;

	UPROPERTY(Category = "Damage", BlueprintReadOnly)
	FReceivedDamageData LastAppliedDamageData;

	UPROPERTY(Category = "Damage", BlueprintReadOnly, EditAnywhere)
	float ExplosionStumbleMinDuration;
//~ End Damage
	
//~ Begin Input
public:
	
	UFUNCTION(Category = "Input", BlueprintCallable)
	bool IsInputPressed(int32 InInputID) const;

	UFUNCTION(Category = "Input", BlueprintCallable, meta = (DisplayName = "Is Input Pressed (Enum)"))
	bool IsInputPressedByEnum(EScWAbilityInputID InInputID) const;

	UFUNCTION(Category = "Input", BlueprintCallable, meta = (DisplayName = "Press Input (Enum)"))
	void PressInputByEnum(EScWAbilityInputID InInputID);

	UFUNCTION(Category = "Input", BlueprintCallable, meta = (DisplayName = "Release Input (Enum)"))
	void ReleaseInputByEnum(EScWAbilityInputID InInputID);

	UFUNCTION(Category = "Input", BlueprintCallable)
	void ForceReleaseAllInputs();

	/**
	*	Direct Input state replication.
	*	These will be called if bReplicateInputDirectly is true on the WaitInput task's owner ability
	*	and is generally not a good thing to use. (Instead, prefer to use Generic Replicated Events).
	*/
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetInputPressedFromWaitInputTask(int32 InInputID);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetInputReleasedFromWaitInputTask(int32 InInputID);

	UPROPERTY(Category = "Input", BlueprintAssignable)
	FInputEventSignature OnInputPressedDelegate;
	
	UPROPERTY(Category = "Input", BlueprintAssignable)
	FInputEventSignature OnInputReleasedDelegate;

protected:
	virtual void AbilityLocalInputPressed(int32 InInputID) override; // UAbilitySystemComponent
	virtual void AbilityLocalInputReleased(int32 InInputID) override; // UAbilitySystemComponent
	
	UPROPERTY()
	TSet<int32> PressedInputIDSet;
//~ End Input

//~ Begin Combo
public:

	UFUNCTION(Category = "Combo", BlueprintCallable)
	bool IsUsingComboSystem() const { return !AvailableCombos.IsEmpty(); }

	UFUNCTION(Category = "Combo", BlueprintCallable)
	const class UScWComboData* GetRelevantCombo() const { return RelevantCombo; }

	UFUNCTION(Category = "Combo | Queue", BlueprintCallable, meta = (AdvancedDisplay = "InUpdateRelevantCombo, InResetIfNoRelevantCombo"))
	void QueueComboMove(const class UScWComboMoveData* InComboMoveData);

	UFUNCTION(Category = "Combo | Queue", BlueprintCallable)
	void AcceptQueuedComboMove();

	UFUNCTION(Category = "Combo | Queue", BlueprintCallable)
	void DenyQueuedComboMove();

	UPROPERTY(Category = "Combo | Queue", BlueprintAssignable)
	FComboMoveEventSignature OnComboMoveQueuedDelegate;

	UPROPERTY(Category = "Combo | Queue", BlueprintAssignable)
	FComboMoveEventSignature OnQueuedComboMoveAcceptedDelegate;

	UPROPERTY(Category = "Combo | Queue", BlueprintAssignable)
	FComboMoveEventSignature OnQueuedComboMoveDeniedDelegate;

	UFUNCTION(Category = "Combo", BlueprintCallable, meta = (AdvancedDisplay = "InUpdateRelevantCombo, InResetIfNoRelevantCombo"))
	void AddComboMove(const class UScWComboMoveData* InComboMoveData, bool InUpdateRelevantCombo = true, bool InResetIfNoRelevantCombo = true);

	UFUNCTION(Category = "Combo", BlueprintCallable, meta = (AdvancedDisplay = "InUpdateRelevantComboOnResetOrFail"))
	void SetComboState(EComboState InState, bool InUpdateRelevantComboOnResetOrFail = true);

	UFUNCTION(Category = "Combo", BlueprintCallable, meta = (AdvancedDisplay = "InResetIfNoRelevantCombo", KeyWords = "GetComboFromCurrentMoves"))
	bool UpdateRelevantComboFromCurrentMoves(bool InResetIfNoRelevantCombo = true);

	UPROPERTY(Category = "Combo", BlueprintAssignable)
	FComboMoveEventSignature OnComboMoveAddedDelegate;

	UPROPERTY(Category = "Combo", BlueprintAssignable)
	FComboStateChangedSignature OnComboStateChangedDelegate;

protected:

	UPROPERTY(Category = "Combo", BlueprintReadOnly)
	EComboState CurrentComboState;

	UPROPERTY(Category = "Combo", BlueprintReadOnly)
	TObjectPtr<const class UScWComboMoveData> QueuedComboMove;

	UPROPERTY(Category = "Combo", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<const class UScWComboMoveData>> CurrentComboMoves;

	UPROPERTY(Category = "Combo", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<const class UScWComboData>> AvailableCombos;

	UPROPERTY(Category = "Combo", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const class UScWComboData> RelevantCombo;

	UPROPERTY(Category = "Combo", BlueprintAssignable)
	FSuccessSignature OnRelevantComboUpdated;
//~ End Combo
};
