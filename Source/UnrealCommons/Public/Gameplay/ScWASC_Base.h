// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWTypes_Delegates.h"

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
struct FReceiveDamageData
{
	GENERATED_BODY()

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	FHitResult HitResult = FHitResult();

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	TObjectPtr<const UDamageType> DamageType = nullptr;

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> Source = nullptr;

	UPROPERTY(Category = "Data", BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AController> Instigator = nullptr;

	FReceiveDamageData(const FHitResult& InHitResult = FHitResult(), const UDamageType* InDamageType = nullptr, AActor* InSource = nullptr, AController* InInstigator = nullptr)
		: HitResult(InHitResult), DamageType(InDamageType), Source(InSource), Instigator(InInstigator) {}
};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageEventSignature, float, InDamage, const FReceiveDamageData&, InData);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComboStateChangedSignature, EComboState, InNewState);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComboMoveEventSignature, class UScWComboMoveData*, InComboMoveData);

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class UNREALCOMMONS_API UScWASC_Base : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UScWASC_Base();
	
//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable, BlueprintPure)
	static UScWASC_Base* TryGetBaseAtaASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface = true);
//~ End Statics

//~ Begin Initialize
public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override; // UAbilitySystemComponent
protected:
	virtual void OnRegister() override; // UActorComponent
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
	FAttributeChangedSignature OnHealthChangedDelegate;

	UPROPERTY(Category = "Attributes", BlueprintAssignable)
	FAttributeChangedSignature OnMaxHealthChangedDelegate;

	UPROPERTY(Category = "Attributes", BlueprintAssignable)
	FDefaultEventSignature OnDiedDelegate;

protected:

	UPROPERTY(Category = "Attributes", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWAS_Base> AttributeSetClass;

	virtual void OnHealthChanged(const FOnAttributeChangeData& InData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& InData);

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
	void GiveAbilitiesWithLevels(TMap<TSubclassOf<UGameplayAbility>, int32> InAbilitiesWithLevels, TArray<FGameplayAbilitySpecHandle>& OutHandleArray);

	UFUNCTION(Category = "Abilities", BlueprintCallable)
	void ClearAbilities(UPARAM(Ref) TArray<FGameplayAbilitySpecHandle>& InHandleArray, const bool bInResetArray);

	UPROPERTY(Category = "Abilities", BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UGameplayAbility>, int32> SpawnAbilities;

	UPROPERTY(Category = "Abilities", BlueprintReadOnly)
	TArray<FGameplayAbilitySpecHandle> SpawnAbilitiesSpecsHandles;
//~ End Abilities

//~ Begin Effects
public:

	UFUNCTION(Category = "Effects", BlueprintCallable)
	FActiveGameplayEffectHandle TryApplyGameplayEffectByClass(TSubclassOf<UGameplayEffect> InClass, float InDuration = -1.0f, float InMagnitude = 0.0f, float InLevel = 1.0f);

	UFUNCTION(Category = "Effects", BlueprintCallable)
	int32 TryRemoveGameplayEffectByClass(TSubclassOf<UGameplayEffect> InClass, int32 InStacksToRemove = -1);

	UFUNCTION(Category = "Effects", BlueprintCallable)
	void ApplySpawnEffect();

	UPROPERTY(Category = "Effects", BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> SpawnEffectClass;

//~ End Effects

//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintCallable)
	const UDamageType* GetLastReceivedDamageType() const { return LastReceivedDamageType; }

	UPROPERTY(Category = "Damage", BlueprintAssignable)
	FDamageEventSignature OnDamageApplied;

	UPROPERTY(Category = "Damage", BlueprintAssignable)
	FDamageEventSignature OnDamageBlocked;

	UPROPERTY(Category = "Damage", BlueprintAssignable)
	FDamageEventSignature OnDamageEvaded;

protected:

	UFUNCTION()
	void OnAvatarTakePointDamage(AActor* InDamagedActor, float InDamage, AController* InInstigator, FVector InHitLocation, UPrimitiveComponent* InHitComponent, FName InBoneName, FVector InHitDirection, const UDamageType* InDamageType, AActor* InDamageCauser);

	UFUNCTION()
	void OnAvatarTakeRadialDamage(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, FVector InOrigin, const FHitResult& InHitResult, AController* InInstigator, AActor* InDamageCauser);

	bool HandleTryReceiveDamage(float InDamage, const FReceiveDamageData& InData);

	FORCEINLINE bool ShouldIgnoreAnyAttackFrom(AController* InInstigator) const;
	FORCEINLINE bool TryIgnoreDamage(float& InOutAdjustedDamage, const FReceiveDamageData& InData);
	FORCEINLINE bool TryBlockDamage(float& InOutAdjustedDamage, const FReceiveDamageData& InData);
	FORCEINLINE bool TryEvadeDamage(float& InOutAdjustedDamage, const FReceiveDamageData& InData);
	FORCEINLINE bool TryApplyDamage(float InDamage, const FReceiveDamageData& InData);

	virtual void PostBlockDamage(float InDamage, const FReceiveDamageData& InData) {}
	virtual void PostEvadeDamage(float InDamage, const FReceiveDamageData& InData) {}
	virtual void PostApplyDamage(float InDamage, const FReceiveDamageData& InData) {}

	//virtual float AdjustIncomingDamage(float InDamage, const struct FReceiveDamageData& InData) const;

	UPROPERTY(Category = "Damage", BlueprintReadOnly)
	TObjectPtr<const UDamageType> LastReceivedDamageType;

	UPROPERTY(Category = "Damage", BlueprintReadOnly, EditAnywhere)
	float ExplosionStumbleMinDuration;
//~ End Damage

//~ Begin Combo
public:

	UFUNCTION(Category = "Combo", BlueprintCallable)
	bool IsUsingComboSystem() const { return !AvailableCombos.IsEmpty(); }

	UFUNCTION(Category = "Combo | Queue", BlueprintCallable, meta = (AdvancedDisplay = "InUpdateRelevantCombo, InResetIfNoRelevantCombo"))
	void QueueComboMove(class UScWComboMoveData* InComboMoveData);

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
	void AddComboMove(class UScWComboMoveData* InComboMoveData, bool InUpdateRelevantCombo = true, bool InResetIfNoRelevantCombo = true);

	UFUNCTION(Category = "Combo", BlueprintCallable, meta = (AdvancedDisplay = "InUpdateRelevantComboOnResetOrFail"))
	void SetComboState(EComboState InState, bool InUpdateRelevantComboOnResetOrFail = true);

	UFUNCTION(Category = "Combo", BlueprintCallable, meta = (AdvancedDisplay = "InResetIfNoRelevantCombo", KeyWords = "GetComboFromCurrentMoves"))
	bool UpdateRelevantComboFromCurrentMoves(bool InResetIfNoRelevantCombo = true);

	UPROPERTY(Category = "Combo", BlueprintReadOnly)
	EComboState CurrentComboState;

	UPROPERTY(Category = "Combo", BlueprintReadOnly)
	TObjectPtr<class UScWComboMoveData> QueuedComboMove;

	UPROPERTY(Category = "Combo", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class UScWComboMoveData>> CurrentComboMoves;

	UPROPERTY(Category = "Combo", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class UScWComboData>> AvailableCombos;

	UPROPERTY(Category = "Combo", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UScWComboData> RelevantCombo;

	UPROPERTY(Category = "Combo", BlueprintAssignable)
	FComboMoveEventSignature OnComboMoveAddedDelegate;

	UPROPERTY(Category = "Combo", BlueprintAssignable)
	FComboStateChangedSignature OnComboStateChangedDelegate;

	UPROPERTY(Category = "Combo", BlueprintAssignable)
	FBoolSignature OnRelevantComboUpdated;
//~ End Combo
};
