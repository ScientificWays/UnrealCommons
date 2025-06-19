// Scientific Ways

#include "Gameplay/ScWASC_Base.h"

#include "Perception/AISenseConfig_Damage.h"

#include "Gameplay/ScWAS_Base.h"
#include "Gameplay/ScWDamageType.h"
#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/DataAssets/ScWComboData.h"
#include "Gameplay/DataAssets/ScWComboMoveData.h"
#include "Gameplay/ScWGameplayFunctionLibrary.h"

UScWASC_Base::UScWASC_Base()
{
	AttributeSetClass = UScWAS_Base::StaticClass();

	bEnableAccumulatedAppliedDamage = true;
	AccumulatedAppliedDamageResetTime = 1.5f;
	bShouldDieOnZeroHealth = true;
}

//~ Begin Statics
UScWASC_Base* UScWASC_Base::TryGetBaseAtaASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface)
{
	return Cast<UScWASC_Base>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor, bInTryFindComponentIfNoInterface));
}
//~ End Statics

//~ Begin Initialize
void UScWASC_Base::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) // UAbilitySystemComponent
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		return;
	}
	// TODO: Check if something was bound before
	InAvatarActor->OnTakePointDamage.AddDynamic(this, &UScWASC_Base::OnAvatarTakePointDamage);
	InAvatarActor->OnTakeRadialDamage.AddDynamic(this, &UScWASC_Base::OnAvatarTakeRadialDamage);

	//OwnerController = Cast<AController>(InOwnerActor);

	//OwnerPlayerController = Cast<AIDPlayerController>(InOwnerActor);
	//check(OwnerController);
}

void UScWASC_Base::DestroyActiveState() // UAbilitySystemComponent
{
	ResolveAccumulatedAppliedDamage();

	Super::DestroyActiveState();
}

void UScWASC_Base::OnRegister() // UActorComponent
{
	Super::OnRegister();

	BaseAS = Cast<UScWAS_Base>(GetOrCreateAttributeSubobject(AttributeSetClass));

	if (!BaseAS)
	{
		return;
	}
	HealthChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(BaseAS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
	MaxHealthChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(BaseAS->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthChanged);
}

void UScWASC_Base::BeginPlay() // UActorComponent
{
	Super::BeginPlay();

	if (IsOwnerActorAuthoritative())
	{
		ApplySpawnEffect();
		GiveSpawnAbilities();
	}
}
//~ End Initialize

//~ Begin Attributes
float UScWASC_Base::GetHealth() const
{
	if (BaseAS)
	{
		return BaseAS->GetHealth();
	}
	return 0.0f;
}

float UScWASC_Base::GetMaxHealth() const
{
	if (BaseAS)
	{
		return BaseAS->GetMaxHealth();
	}
	return 0.0f;
}

bool UScWASC_Base::IsFullHealth() const
{
	if (BaseAS)
	{
		return BaseAS->GetHealth() == BaseAS->GetMaxHealth();
	}
	return false;
}

void UScWASC_Base::SetHealth(float InHealth)
{
	if (TSubclassOf<UGameplayEffect> SetHealthEffectClass = UScWGameplayFunctionLibrary::GetSetHealthGameplayEffectClass(this))
	{
		TryApplyGameplayEffectByClass(SetHealthEffectClass, -1.0f, InHealth);
	}
	else
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("UScWASC_Base::SetHealth() SetHealthEffectClass == nullptr! Can't set health."));
	}
}

void UScWASC_Base::AddHealth(float InHealth)
{
	if (TSubclassOf<UGameplayEffect> AddHealthEffectClass = UScWGameplayFunctionLibrary::GetAddHealthGameplayEffectClass(this))
	{
		TryApplyGameplayEffectByClass(AddHealthEffectClass, -1.0f, InHealth);
	}
	else
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("UScWASC_Base::AddHealth() AddHealthEffectClass == nullptr! Can't add health."));
	}
}

void UScWASC_Base::OnHealthChanged(const FOnAttributeChangeData& InData)
{
	OnHealthChangedDelegate.Broadcast(InData.Attribute, InData.OldValue, InData.NewValue);

	if (InData.NewValue == 0.0f)
	{
		OnZeroHealth();
	}
}

void UScWASC_Base::OnMaxHealthChanged(const FOnAttributeChangeData& InData)
{
	OnMaxHealthChangedDelegate.Broadcast(InData.Attribute, InData.OldValue, InData.NewValue);
}

void UScWASC_Base::OnZeroHealth()
{
	if (bShouldDieOnZeroHealth)
	{
		HandleDied();
	}
}

void UScWASC_Base::HandleDied()
{
	OnDiedDelegate.Broadcast();
}
//~ End Attributes

//~ Begin Abilities
void UScWASC_Base::GiveSpawnAbilities()
{
	ensure(SpawnAbilitiesSpecsHandles.IsEmpty());
	GiveAbilitiesFromGiveData(SpawnAbilitiesGiveData, SpawnAbilitiesSpecsHandles);
}

void UScWASC_Base::GiveAbilitiesFromGiveData(TArray<FScWGameplayGiveAbilityData> InAbilitiesGiveData, TArray<FGameplayAbilitySpecHandle>& OutHandleArray)
{
	for (const FScWGameplayGiveAbilityData& SampleGiveData : InAbilitiesGiveData)
	{
		FGameplayAbilitySpec SampleHandle = FGameplayAbilitySpec(SampleGiveData.Class, SampleGiveData.Level);

		if (SampleGiveData.bAssignInputID)
		{
			SampleHandle.InputID = static_cast<int32>(SampleGiveData.InputID);
		}
		OutHandleArray.Add(GiveAbility(SampleHandle));
	}
}

void UScWASC_Base::ClearAbilities(TArray<FGameplayAbilitySpecHandle>& InHandleArray, const bool bInResetArray)
{
	for (auto SampleHandle : InHandleArray)
	{
		ClearAbility(SampleHandle);
	}
	if (bInResetArray)
	{
		InHandleArray.Empty();
	}
}

void UScWASC_Base::GetAbilitiesByInput(EScWAbilityInputID InInputID, TArray<UGameplayAbility*>& OutAbilityArray) const
{
	OutAbilityArray.Empty();

	for (const FGameplayAbilitySpec& SampleSpec : GetActivatableAbilities())
	{
		if (SampleSpec.InputID != static_cast<int32>(InInputID))
		{
			continue;
		}
		AppendAbilityInstancesOrCDO(SampleSpec, OutAbilityArray);
	}
}

void UScWASC_Base::SetInputAbilities(const EScWAbilityInputID InInputID, const TArray<TSubclassOf<UGameplayAbility>>& InClassArray)
{
	ClearAllAbilitiesWithInputID(static_cast<int32>(InInputID));

	for (const TSubclassOf<UGameplayAbility>& SampleClass : InClassArray)
	{
		AddInputAbility(InInputID, SampleClass);
	}
}

FGameplayAbilitySpecHandle UScWASC_Base::AddInputAbility(const EScWAbilityInputID InInputID, const TSubclassOf<UGameplayAbility> InClass)
{
	if (InClass)
	{
		FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(InClass, 1, static_cast<int32>(InInputID));
		return GiveAbility(AbilitySpec);
	}
	return FGameplayAbilitySpecHandle();
}

template<class T>
void UScWASC_Base::AppendAbilityInstancesOrCDO(const FGameplayAbilitySpec& InSpec, TArray<T*>& OutArray) const
{
	TArray<UGameplayAbility*> SampleInstanceArray = InSpec.GetAbilityInstances();

	if (SampleInstanceArray.Num())
	{
		for (UGameplayAbility* SampleInstance : SampleInstanceArray)
		{
			if (T* SampleAbility = Cast<T>(SampleInstance))
			{
				OutArray.Add(SampleAbility);
			}
		}
	}
	else
	{
		if (T* SampleAbilityCDO = Cast<T>(InSpec.Ability))
		{
			OutArray.Add(SampleAbilityCDO);
		}
	}
}
//~ End Abilities

//~ Begin Effects
FActiveGameplayEffectHandle UScWASC_Base::TryApplyGameplayEffectByClass(TSubclassOf<UGameplayEffect> InClass, float InDuration, float InMagnitude, float InLevel)
{
	if (InClass == nullptr)
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("UScWASC_Base::TryApplyGameplayEffectByClass() InClass == nullptr! Can't apply GameplayEffect by class."));
		return FActiveGameplayEffectHandle();
	}
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(InClass, InLevel, MakeEffectContext());

	if (InDuration > 0.0f)
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(FScWGameplayTags::SetByCaller_Duration, InDuration);
	}
	EffectSpecHandle.Data->SetSetByCallerMagnitude(FScWGameplayTags::SetByCaller_Magnitude, InMagnitude);
	return ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}

int32 UScWASC_Base::TryRemoveGameplayEffectByClass(TSubclassOf<UGameplayEffect> InClass, int32 InStacksToRemove)
{
	if (InClass == nullptr)
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("UScWASC_Base::TryRemoveGameplayEffectByClass() InClass == nullptr! Can't remove GameplayEffect by class."));
		return 0;
	}
	FGameplayEffectQuery RemoveQuery = FGameplayEffectQuery();
	RemoveQuery.EffectDefinition = InClass;
	return RemoveActiveEffects(RemoveQuery, InStacksToRemove);
}

void UScWASC_Base::ApplySpawnEffect()
{
	if (SpawnEffectClass)
	{
		TryApplyGameplayEffectByClass(SpawnEffectClass);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s UScWASC_Base::ApplySpawnEffect() Invalid SpawnEffectClass! Possibly OnZeroHealth() will be called."), *GetName()));
	}
}
//~ End Effects

//~ Begin Damage
#define ACCUMULATED_DAMAGE_DECLARE_METHODS(InRoute) \
void UScWASC_Base::Accumulate##InRoute##Damage(float InDamage, bool bInAutoResolveNextTick) \
{ \
	if (bEnableAccumulated##InRoute##Damage) \
	{ \
		Accumulated##InRoute##Damage += InDamage; \
		RequestResolveAccumulated##InRoute##DamageNextTick(); \
	} \
} \
void UScWASC_Base::RequestResolveAccumulated##InRoute##DamageNextTick() \
{ \
	if (!Accumulated##InRoute##DamageTimerHandle.IsValid()) \
	{ \
		if (UWorld* World = GetWorld()) \
		{ \
			FTimerManager& WorldTimerManager = World->GetTimerManager(); \
			check(!WorldTimerManager.TimerExists(Accumulated##InRoute##DamageTimerHandle)); \
			Accumulated##InRoute##DamageTimerHandle = WorldTimerManager.SetTimerForNextTick(this, &UScWASC_Base::ResolveAccumulated##InRoute##DamageFromNextTickTimer); \
			if (Accumulated##InRoute##DamageResetTime > 0.0f) \
				WorldTimerManager.SetTimer(Accumulated##InRoute##DamageResetTimerHandle, this, &UScWASC_Base::ResetAccumulated##InRoute##Damage, Accumulated##InRoute##DamageResetTime); \
		} \
	} \
} \
void UScWASC_Base::ResolveAccumulated##InRoute##DamageFromNextTickTimer() \
{ \
	ResolveAccumulated##InRoute##Damage(); \
	if (Accumulated##InRoute##DamageResetTime <= 0.0f) \
		ResetAccumulated##InRoute##Damage(); \
} \
void UScWASC_Base::ResolveAccumulated##InRoute##Damage() \
{ \
	if (UWorld* World = GetWorld()) \
		World->GetTimerManager().ClearTimer(Accumulated##InRoute##DamageTimerHandle); \
	if (Accumulated##InRoute##Damage != 0.0f) \
		OnAccumulated##InRoute##DamageResolved.Broadcast(Accumulated##InRoute##Damage); \
} \
void UScWASC_Base::ResetAccumulated##InRoute##Damage() \
{ \
	Accumulated##InRoute##Damage = 0.0f; \
}

//ACCUMULATED_DAMAGE_DECLARE_METHODS(Ignored)
//ACCUMULATED_DAMAGE_DECLARE_METHODS(Blocked)
//ACCUMULATED_DAMAGE_DECLARE_METHODS(Evaded)
ACCUMULATED_DAMAGE_DECLARE_METHODS(Applied)

#undef ACCUMULATED_DAMAGE_DECLARE_METHODS

void UScWASC_Base::OnAvatarTakePointDamage(AActor* InDamagedActor, float InDamage, AController* InInstigator, FVector InHitLocation, UPrimitiveComponent* InHitComponent, FName InBoneName, FVector InHitDirection, const UDamageType* InDamageType, AActor* InDamageCauser)
{
	HandleTryReceiveDamage(InDamage, { FHitResult(InDamagedActor, InHitComponent, InHitLocation, -InHitDirection), InDamageType, InDamageCauser, InInstigator });
}

void UScWASC_Base::OnAvatarTakeRadialDamage(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, FVector InOrigin, const FHitResult& InHitResult, AController* InInstigator, AActor* InDamageCauser)
{
	HandleTryReceiveDamage(InDamage, { InHitResult, InDamageType, InDamageCauser, InInstigator });
}

bool UScWASC_Base::HandleTryReceiveDamage(float InDamage, const FReceiveDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	UAISense_Damage::ReportDamageEvent(this, GetAvatarActor(), InData.Instigator, InDamage, InData.Source->GetActorLocation(), InData.HitResult.Location);

	if (TryIgnoreDamage(InDamage, InData))
	{
		OnDamageIgnored.Broadcast(InDamage, InData);
		PostIgnoreDamage(InDamage, InData);
		return false;
	}
	else if (TryBlockDamage(InDamage, InData))
	{
		OnDamageBlocked.Broadcast(InDamage, InData);
		PostBlockDamage(InDamage, InData);
		return false;
	}
	else if (TryEvadeDamage(InDamage, InData))
	{
		OnDamageEvaded.Broadcast(InDamage, InData);
		PostEvadeDamage(InDamage, InData);
		return false;
	}
	else if (TryApplyDamage(InDamage, InData))
	{
		OnDamageApplied.Broadcast(InDamage, InData);
		PostApplyDamage(InDamage, InData);
		return true;
	}
	else
	{
		return false;
	}
}

bool UScWASC_Base::ShouldIgnoreAnyAttackFrom(AController* InInstigator) const
{
	if (InInstigator == nullptr || InInstigator == GetOwnerActor())
	{
		return false;
	}
	/*if (IDGameState->ShouldIgnoreDamageFromFriendlyBots() && !InInstigator->IsPlayerController())
	{
		if (ITeamInterface* PawnTeamInterface = Cast<ITeamInterface>(OwnerPawn))
		{
			if (ITeamInterface* InstigatorTeamInterface = Cast<ITeamInterface>(InInstigator))
			{
				if (PawnTeamInterface->GetTeamName() == InstigatorTeamInterface->GetTeamName())
				{
					return true;
				}
			}
		}
	}
	if (HasMatchingGameplayTag(FIDGameplayTags::State_Captured))
	{
		if (ITeamInterface* PawnTeamInterface = Cast<ITeamInterface>(OwnerPawn))
		{
			if (ITeamInterface* InstigatorTeamInterface = Cast<ITeamInterface>(InInstigator))
			{
				if (PawnTeamInterface->GetTeamName() == InstigatorTeamInterface->GetTeamName())
				{
					return true;
				}
			}
		}
	}*/
	return false;
}

bool UScWASC_Base::TryIgnoreDamage(float& InOutAdjustedDamage, const FReceiveDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	bool bIsIgnored = false;
	/*if (HasMatchingGameplayTag(FIDGameplayTags::Effect_IgnoreAttack_Bullet) && InData.DamageTypeClass->IsChildOf(UIDDamageType_Bullet::StaticClass()))
	{
		bIsIgnored = true;
	}
	else if (HasMatchingGameplayTag(FIDGameplayTags::Effect_IgnoreAttack_Melee) && InData.DamageTypeClass->IsChildOf(UIDDamageType_Melee::StaticClass()))
	{
		bIsIgnored = true;
	}
	else */if (ShouldIgnoreAnyAttackFrom(InData.Instigator))
	{
		bIsIgnored = true;
	}
	if (bIsIgnored)
	{
		//AccumulateIgnoredDamage(InOutAdjustedDamage);
		return true;
	}
	else
	{
		return false;
	}
}

bool UScWASC_Base::TryBlockDamage(float& InOutAdjustedDamage, const FReceiveDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	bool bIsBlocked = false;
	/*if (HasMatchingGameplayTag(FIDGameplayTags::Effect_BlockAttack_Bullet) && InData.DamageTypeClass->IsChildOf(UIDDamageType_Bullet::StaticClass()))
	{
		bIsBlocked = true;
	}
	else if (HasMatchingGameplayTag(FIDGameplayTags::Effect_BlockAttack_Melee) && InData.DamageTypeClass->IsChildOf(UIDDamageType_Melee::StaticClass()))
	{
		bIsBlocked = true;
	}*/
	if (bIsBlocked)
	{
		//AccumulateBlockedDamage(InOutAdjustedDamage);
		return true;
	}
	else
	{
		return false;
	}
}

bool UScWASC_Base::TryEvadeDamage(float& InOutAdjustedDamage, const FReceiveDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	const UScWAS_Base* AttributeSet = GetAttributeSet<UScWAS_Base>();
	check(AttributeSet);

	float EvasionChance = 0.0f;

	/*if (InData.DamageType->IsA(UScWDT_Ranged::StaticClass()))
	{
		EvasionChance = AttributeSet->GetRangedDamageEvasionChance();
	}
	else if (InData.DamageType->IsA(UScWDT_Melee::StaticClass()))
	{
		EvasionChance = AttributeSet->GetMeleeDamageEvasionChance();
	}*/
	if (FMath::FRand() <= EvasionChance)
	{
		//AccumulateEvadedDamage(InOutAdjustedDamage);
		return true;
	}
	else
	{
		return false;
	}
}

bool UScWASC_Base::TryApplyDamage(float InDamage, const FReceiveDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	//InDamage = AdjustIncomingDamage(InDamage, InData);

	if (InDamage <= 0.0f)
	{
		return false;
	}
	LastReceivedDamageType = InData.DamageType;

	/*if (UIDDamageType_Explosion* ExplosionDamageType = Cast<UIDDamageType_Explosion>(InData.DamageTypeClass.GetDefaultObject()))
	{
		if (ExplosionDamageType->StumbleEffectClass && ExplosionDamageType->DamageToStumbleDurationCurve)
		{
			float ExplosionStumbleDuration = ExplosionDamageType->DamageToStumbleDurationCurve->GetFloatValue(InDamage);
			if (ExplosionStumbleDuration >= ExplosionStumbleMinDuration)
			{
				TryApplyGameplayEffectByClass(ExplosionDamageType->StumbleEffectClass, ExplosionStumbleDuration);
			}
		}
	}*/
	const UScWDamageType* ATADamageType = Cast<UScWDamageType>(InData.DamageType);

	TSubclassOf<UGameplayEffect> ApplyDamageGameplayEffectClass = UScWGameplayFunctionLibrary::GetApplyDamageGameplayEffectClassForType(this, InData.DamageType);

	if (ApplyDamageGameplayEffectClass)
	{
		AccumulateAppliedDamage(InDamage);

		FGameplayEffectContextHandle DamageEffectContext = MakeEffectContext();
		DamageEffectContext.AddSourceObject(this);
		DamageEffectContext.AddInstigator(InData.Instigator, InData.Source);

		float PrevHealth = GetHealth();

		FGameplayEffectSpecHandle DamageEffectHandle = MakeOutgoingSpec(ApplyDamageGameplayEffectClass, 1.0f, DamageEffectContext);
		DamageEffectHandle.Data->SetSetByCallerMagnitude(FScWGameplayTags::SetByCaller_Magnitude, -InDamage);
		FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectSpecToSelf(*DamageEffectHandle.Data.Get());

		if (EffectHandle.WasSuccessfullyApplied())
		{
			if (InDamage > PrevHealth)
			{
				/*if (IPawnInterface* OwnerPawnInterface = Cast<IPawnInterface>(OwnerPawn))
				{
					OwnerPawnInterface->HandleZeroHealthDamageRemainder(InDamage - PrevHealth, InData);
				}*/
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		UE_LOG(LogAbilitySystemComponent, Log, TEXT("UScWASC_Base::TryApplyDamage() DamageEffectClass == nullptr! Can't apply damage."));
		return false;
	}
}

/*float UScWASC_Base::AdjustIncomingDamage(float InDamage, const FReceiveDamageData& InData) const
{
	bool bApplyBoneDamageMul = true;
	bool bApplyTeamDamageMul = true;

	if (CustomBehavior && CustomBehavior->IsAdjustIncomingDamageImplemented())
	{
		InDamage = CustomBehavior->BP_AdjustIncomingDamage(this, InDamage, InData, bApplyBoneDamageMul, bApplyTeamDamageMul);
	}
	if (bApplyBoneDamageMul && !InData.HitResult.BoneName.IsNone())
	{
		InDamage *= IDGameState->BP_GetBoneDamageMul(InData.HitResult.BoneName);
	}
	if (bApplyTeamDamageMul)
	{
		if (ITeamInterface* PawnTeamInterface = Cast<ITeamInterface>(OwnerPawn))
		{
			if (ITeamInterface* InstigatorTeamInterface = Cast<ITeamInterface>(InData.Instigator))
			{
				const FTeamIdentity& PawnTeam = PawnTeamInterface->GetTeamIdentity();
				const FTeamIdentity& InstigatorTeam = InstigatorTeamInterface->GetTeamIdentity();
				InDamage *= IDGameState->BP_GetTeamDamageMul(InstigatorTeam, PawnTeam);
			}
		}
	}
	return IDGameState->BP_AdjustIncomingDamage(OwnerPawn, InDamage, InData);
}*/
//~ End Damage

//~ Begin Input
bool UScWASC_Base::IsInputPressed(int32 InInputID) const
{
	return PressedInputIDSet.Contains(InInputID);
}

bool UScWASC_Base::IsInputPressedByEnum(EScWAbilityInputID InInputID) const
{
	return PressedInputIDSet.Contains(static_cast<int32>(InInputID));
}

void UScWASC_Base::PressInputByEnum(EScWAbilityInputID InInputID)
{
	PressInputID(static_cast<int32>(InInputID));
}

void UScWASC_Base::ReleaseInputByEnum(EScWAbilityInputID InInputID)
{
	ReleaseInputID(static_cast<int32>(InInputID));
}

void UScWASC_Base::ForceReleaseAllInputs()
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& SampleSpec : ActivatableAbilities.Items)
	{
		SampleSpec.InputPressed = false;
		if (SampleSpec.Ability && SampleSpec.IsActive())
		{
			if (SampleSpec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
			{
				ServerSetInputReleased(SampleSpec.Handle);
			}
			AbilitySpecInputReleased(SampleSpec);

PRAGMA_DISABLE_DEPRECATION_WARNINGS
			// Fixing this up to use the instance activation, but this function should be deprecated as it cannot work with InstancedPerExecution
			UE_CLOG(SampleSpec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerExecution, LogScWGameplay, Warning, TEXT("%hs: %s is InstancedPerExecution. This is unreliable for Input as you may only interact with the latest spawned Instance"), __func__, *GetNameSafe(SampleSpec.Ability));
			TArray<UGameplayAbility*> Instances = SampleSpec.GetAbilityInstances();
			const FGameplayAbilityActivationInfo& ActivationInfo = Instances.IsEmpty() ? SampleSpec.ActivationInfo : Instances.Last()->GetCurrentActivationInfoRef();
PRAGMA_ENABLE_DEPRECATION_WARNINGS

			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, SampleSpec.Handle, ActivationInfo.GetActivationPredictionKey());
		}
	}
}

bool UScWASC_Base::Server_SetInputPressedFromWaitInputTask_Validate(int32 InInputID)
{
	return true;
}

void UScWASC_Base::Server_SetInputPressedFromWaitInputTask_Implementation(int32 InInputID)
{
	AbilityLocalInputPressed(InInputID);
}

bool UScWASC_Base::Server_SetInputReleasedFromWaitInputTask_Validate(int32 InInputID)
{
	return true;
}

void UScWASC_Base::Server_SetInputReleasedFromWaitInputTask_Implementation(int32 InInputID)
{
	AbilityLocalInputReleased(InInputID);
}

void UScWASC_Base::AbilityLocalInputPressed(int32 InInputID) // UAbilitySystemComponent
{
	PressedInputIDSet.Add(InInputID);
	Super::AbilityLocalInputPressed(InInputID);
	OnInputPressedDelegate.Broadcast(InInputID);
}

void UScWASC_Base::AbilityLocalInputReleased(int32 InInputID) // UAbilitySystemComponent
{
	PressedInputIDSet.Remove(InInputID);
	Super::AbilityLocalInputReleased(InInputID);
	OnInputReleasedDelegate.Broadcast(InInputID);
}
//~ End Input

//~ Begin Combo
void UScWASC_Base::QueueComboMove(UScWComboMoveData* InComboMoveData)
{
	if (InComboMoveData)
	{
		QueuedComboMove = InComboMoveData;
		OnComboMoveQueuedDelegate.Broadcast(QueuedComboMove);
	}
}

void UScWASC_Base::AcceptQueuedComboMove()
{
	if (QueuedComboMove)
	{
		UScWComboMoveData* AcceptedComboMove = QueuedComboMove;
		QueuedComboMove = nullptr;
		OnQueuedComboMoveAcceptedDelegate.Broadcast(AcceptedComboMove);

		AddComboMove(AcceptedComboMove, true, true);
	}
}

void UScWASC_Base::DenyQueuedComboMove()
{
	if (QueuedComboMove)
	{
		UScWComboMoveData* DeniedComboMove = QueuedComboMove;
		QueuedComboMove = nullptr;
		OnQueuedComboMoveDeniedDelegate.Broadcast(DeniedComboMove);

	}
}

void UScWASC_Base::AddComboMove(UScWComboMoveData* InComboMoveData, bool InUpdateRelevantCombo, bool InResetIfNoRelevantCombo)
{
	CurrentComboMoves.Add(InComboMoveData);
	OnComboMoveAddedDelegate.Broadcast(InComboMoveData);

	if (InUpdateRelevantCombo)
	{
		bool IsNewComboRelevant = UpdateRelevantComboFromCurrentMoves(InResetIfNoRelevantCombo);

		if (InResetIfNoRelevantCombo && !IsNewComboRelevant) // Probably went full cycle, so try to restart combo
		{
			AddComboMove(InComboMoveData, InUpdateRelevantCombo, false);
		}
	}
}

void UScWASC_Base::SetComboState(EComboState InState, bool InUpdateRelevantComboOnResetOrFail)
{
	if (CurrentComboState != InState)
	{
		CurrentComboState = InState;

		switch (CurrentComboState)
		{
			case EComboState::Reset:
			{
				CurrentComboMoves.Empty();

				if (InUpdateRelevantComboOnResetOrFail && !AvailableCombos.IsEmpty())
				{
					UpdateRelevantComboFromCurrentMoves(false);
				}
				break;
			}
			case EComboState::ReadyForMove:
			{
				break;
			}
			case EComboState::NotYetReadyForMove:
			{
				break;
			}
			case EComboState::Failed:
			{
				CurrentComboMoves.Empty();

				if (InUpdateRelevantComboOnResetOrFail && !AvailableCombos.IsEmpty())
				{
					UpdateRelevantComboFromCurrentMoves(false);
				}
				break;
			}
			default:
			{
				checkNoEntry();
				break;
			}
		}
		OnComboStateChangedDelegate.Broadcast(CurrentComboState);
	}
}

bool UScWASC_Base::UpdateRelevantComboFromCurrentMoves(bool InResetIfNoRelevantCombo)
{
	auto PrevRelevantCombo = RelevantCombo;
	RelevantCombo = nullptr;

	bool OutSuccess = false;

	ensure(IsUsingComboSystem());
	for (auto SampleCombo : AvailableCombos)
	{
		if (SampleCombo->Moves == CurrentComboMoves)
		{
			RelevantCombo = SampleCombo;
			OutSuccess = true;
			break;
		}
	}
	if (RelevantCombo != PrevRelevantCombo)
	{
		OnRelevantComboUpdated.Broadcast(OutSuccess);
	}
	if (InResetIfNoRelevantCombo && !OutSuccess)
	{
		if (CurrentComboState != EComboState::Reset)
		{
			SetComboState(EComboState::Reset, false);
		}
		else
		{
			CurrentComboMoves.Empty();
		}
	}
	return OutSuccess;
}
//~ End Combo
