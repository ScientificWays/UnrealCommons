// Scientific Ways

#include "Gameplay/ScWASC_Base.h"

#include "Gameplay/Characters/ScWCharacterData.h"

#include "Gameplay/ScWDamageType.h"
#include "Gameplay/ScWGameplayTags.h"
#include "Gameplay/Combo/ScWComboData.h"
#include "Gameplay/ScWASC_InitInterface.h"
#include "Gameplay/Attributes/ScWAS_Base.h"
#include "Gameplay/Combo/ScWComboMoveData.h"
#include "Gameplay/ScWGameplayFunctionLibrary.h"

#include "Perception/AISenseConfig_Damage.h"

class UScWASC_Base* FReceivedDamageData::TryGetAttackerBaseASC() const
{
	UScWASC_Base* OutASC = nullptr;

	if (UScWASC_Base* SourceASC = UScWASC_Base::TryGetBaseScWASCFromActor(Source))
	{
		OutASC = SourceASC;
	}
	else if (Instigator)
	{
		if (UScWASC_Base* InstigatorASC = UScWASC_Base::TryGetBaseScWASCFromActor(Instigator))
		{
			OutASC = InstigatorASC;
		}
		else if (UScWASC_Base* InstigatorPawnASC = UScWASC_Base::TryGetBaseScWASCFromActor(Instigator->GetPawn()))
		{
			OutASC = InstigatorPawnASC;
		}
	}
	return OutASC;
}

FString FReceivedDamageData::ToAnalyticsString() const
{
	FString OutString;
	OutString += FString::Printf(TEXT("DamageType:%s;"), DamageType ? *DamageType->AnalyticsName : TEXT("None"));
	OutString += FString::Printf(TEXT("Source:%s;"), Source ? *Source->GetName() : TEXT("None"));
	OutString += FString::Printf(TEXT("Instigator:%s;"), Instigator ? *Instigator->GetName() : TEXT("None"));
	OutString += FString::Printf(TEXT("HitLocation:%s;"), *GET_ROUNDED_VECTOR_STRING(HitResult.ImpactPoint));
	OutString += FString::Printf(TEXT("HitBone:%s;"), *HitResult.BoneName.ToString());
	return OutString;
}

UScWASC_Base::UScWASC_Base()
{
	AttributeSetClass = UScWAS_Base::StaticClass();

	bEnableAccumulatedAppliedDamage = true;
	AccumulatedAppliedDamageResetTime = 1.5f;
	bShouldDieOnZeroHealth = true;
}

//~ Begin Statics
UScWASC_Base* UScWASC_Base::TryGetBaseScWASCFromActor(const AActor* InActor, bool bInTryFindComponentIfNoInterface)
{
	return Cast<UScWASC_Base>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InActor, bInTryFindComponentIfNoInterface));
}
//~ End Statics

//~ Begin Initialize
void UScWASC_Base::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) // UAbilitySystemComponent
{
	ensureReturn(GetWorld());

	if (GetWorld()->IsGameWorld())
	{
		if (AbilityActorInfo.IsValid())
		{
			if (AbilityActorInfo->AvatarActor.IsValid())
			{
				AbilityActorInfo->AvatarActor->OnTakePointDamage.RemoveDynamic(this, &ThisClass::OnAvatarTakePointDamage);
				AbilityActorInfo->AvatarActor->OnTakeRadialDamage.RemoveDynamic(this, &ThisClass::OnAvatarTakeRadialDamage);
			}
		}
		Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

		ensureIf(InAvatarActor)
		{
			InAvatarActor->OnTakePointDamage.AddDynamic(this, &ThisClass::OnAvatarTakePointDamage);
			InAvatarActor->OnTakeRadialDamage.AddDynamic(this, &ThisClass::OnAvatarTakeRadialDamage);
		}
		IScWASC_InitInterface::HandleInit(this, InOwnerActor, InAvatarActor);
	}
	else
	{
		Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	}
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
	HealthChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(BaseAS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
	MaxHealthChangedDelegateHandle = GetGameplayAttributeValueChangeDelegate(BaseAS->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthAttributeChanged);

	RegisterGameplayTagEvent(FScWGameplayTags::State_Stunned, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnStunnedTagNumChanged);
	RegisterGameplayTagEvent(FScWGameplayTags::Input_Block, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnInputBlockMovementTagNumChanged);
}

void UScWASC_Base::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);

	SpawnEffectClass = InInitCharacterData->SpawnEffectClass;
	SpawnAbilitiesGiveData = InInitCharacterData->DefaultAbilitiesGiveData;
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
		UE_LOG(LogScWGameplay, Warning, TEXT("UScWASC_Base::SetHealth() SetHealthEffectClass == nullptr! Can't set health."));
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
		UE_LOG(LogScWGameplay, Warning, TEXT("UScWASC_Base::AddHealth() AddHealthEffectClass == nullptr! Can't add health."));
	}
}

void UScWASC_Base::OnHealthAttributeChanged(const FOnAttributeChangeData& InData)
{
	OnHealthChanged.Broadcast(InData.Attribute, InData.OldValue, InData.NewValue);

	if (InData.NewValue == 0.0f)
	{
		OnZeroHealth();
	}
}

void UScWASC_Base::OnMaxHealthAttributeChanged(const FOnAttributeChangeData& InData)
{
	OnMaxHealthChanged.Broadcast(InData.Attribute, InData.OldValue, InData.NewValue);
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
	if (TSubclassOf<UGameplayEffect> DeadStateEffectClass = UScWGameplayFunctionLibrary::GetDeadStateGameplayEffectClass(this))
	{
		TryApplyGameplayEffectByClass(DeadStateEffectClass);
	}
	else
	{
		UE_LOG(LogScWGameplay, Warning, TEXT("UScWASC_Base::HandleDied() DeadEffectClass == nullptr! Can't apply dead state."));
	}
	OnDied.Broadcast();
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
		UE_LOG(LogScWGameplay, Warning, TEXT("UScWASC_Base::TryApplyGameplayEffectByClass() InClass == nullptr! Can't apply GameplayEffect by class."));
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
		UE_LOG(LogScWGameplay, Warning, TEXT("UScWASC_Base::TryRemoveGameplayEffectByClass() InClass == nullptr! Can't remove GameplayEffect by class."));
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
		OnSpawnEffectApplied.Broadcast();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s UScWASC_Base::ApplySpawnEffect() Invalid SpawnEffectClass! Possibly OnZeroHealth() will be called."), *GetName()));
	}
}
//~ End Effects

//~ Begin Tags
void UScWASC_Base::OnStunnedTagNumChanged(const FGameplayTag InCallbackTag, int32 InNewNum)
{
	if (InNewNum > 0)
	{
		static const FGameplayTagContainer CancelByStunnedTagContainer = FGameplayTagContainer(FScWGameplayTags::Ability_CancelBy_Stunned);
		CancelAbilities(&CancelByStunnedTagContainer);
	}
}

void UScWASC_Base::OnInputBlockMovementTagNumChanged(const FGameplayTag InCallbackTag, int32 InNewNum)
{
	APawn* AvatarPawn = Cast<APawn>(GetAvatarActor_Direct());
	ensureReturn(AvatarPawn);

	/*if (AScWPlayerController* OwnerPlayerController = AvatarPawn->GetController<AScWPlayerController>())
	{
		if (InNewNum > 0) // HasMatchingGameplayTag(FScWGameplayTags::Input_Block_Movement)
		{
			OwnerPlayerController->AddMovementInputBlockSource(this);
		}
		else
		{
			OwnerPlayerController->RemoveMovementInputBlockSource(this);
		}
	}
	else */if (AController* OwnerController = AvatarPawn->GetController())
	{
		if (InNewNum > 0) // HasMatchingGameplayTag(FScWGameplayTags::Input_Block_Movement)
		{
			OwnerController->SetIgnoreMoveInput(true); // Should be enough to use native stacked state storage feature
		}
		else
		{
			OwnerController->SetIgnoreMoveInput(false); // As long as EGameplayTagEventType::NewOrRemoved is used
		}
	}
	else
	{
		//ensure(false);
	}
}
//~ End Tags

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
	const UScWDamageType* ScWDamageType = Cast<UScWDamageType>(InDamageType);
	ensure(ScWDamageType);
	
	FHitResult MinimalInfoHitResult;
	UScWGameplayFunctionLibrary::MakeMinimalInfoDamageImpactHitResult(InDamageCauser, nullptr, (InInstigator ? InInstigator->GetPawn() : nullptr), InDamagedActor, InHitComponent, MinimalInfoHitResult);
	HandleTryReceiveDamage(InDamage, { MinimalInfoHitResult, ScWDamageType, InDamageCauser, InInstigator });
}

void UScWASC_Base::OnAvatarTakeRadialDamage(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, FVector InOrigin, const FHitResult& InHitResult, AController* InInstigator, AActor* InDamageCauser)
{
	const UScWDamageType* ScWDamageType = Cast<UScWDamageType>(InDamageType);
	ensure(ScWDamageType);
	HandleTryReceiveDamage(InDamage, { InHitResult, ScWDamageType, InDamageCauser, InInstigator });
}

bool UScWASC_Base::HandleTryReceiveDamage(float InDamage, const FReceivedDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	FVector SourceLocation = FVector::ZeroVector;

	if (InData.Source)
	{
		SourceLocation = InData.Source->GetActorLocation();
	}
	else if (InData.Instigator && InData.Instigator->GetPawn())
	{
		SourceLocation = InData.Instigator->GetPawn()->GetActorLocation();
	}
	else if (GetAvatarActor_Direct())
	{
		SourceLocation = GetAvatarActor_Direct()->GetActorLocation();
	}
	UAISense_Damage::ReportDamageEvent(this, GetAvatarActor_Direct(), InData.Instigator, InDamage, SourceLocation, InData.HitResult.Location);

	if (HasMatchingGameplayTag(FScWGameplayTags::Cheat_Invulnerable))
	{
		return false;
	}
	else if (TryIgnoreDamage(InDamage, InData))
	{
		OnDamageIgnored.Broadcast(InDamage, InData);
		return false;
	}
	else if (TryBlockDamage(InDamage, InData))
	{
		OnDamageBlocked.Broadcast(InDamage, InData);
		return false;
	}
	else if (TryEvadeDamage(InDamage, InData))
	{
		OnDamageEvaded.Broadcast(InDamage, InData);
		return false;
	}
	else if (TryApplyDamage(InDamage, InData))
	{
		OnDamageApplied.Broadcast(InDamage, InData);
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

bool UScWASC_Base::TryIgnoreDamage(float& InOutAdjustedDamage, const FReceivedDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	bool bIsIgnored = false;

	if (HasMatchingGameplayTag(FScWGameplayTags::State_IgnoreAnyDamage))
	{
		bIsIgnored = true;
	}
	else
	{
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

bool UScWASC_Base::TryBlockDamage(float& InOutAdjustedDamage, const FReceivedDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	bool bIsBlocked = false;

	if (HasMatchingGameplayTag(FScWGameplayTags::State_BlockAnyDamage))
	{
		bIsBlocked = true;
	}
	else
	{
		// TODO: Move this to respective DamageType classes
		/*if (HasMatchingGameplayTag(FIDGameplayTags::Effect_BlockAttack_Bullet) && InData.DamageTypeClass->IsChildOf(UIDDamageType_Bullet::StaticClass()))
		{
			bIsBlocked = true;
		}
		else if (HasMatchingGameplayTag(FIDGameplayTags::Effect_BlockAttack_Melee) && InData.DamageTypeClass->IsChildOf(UIDDamageType_Melee::StaticClass()))
		{
			bIsBlocked = true;
		}*/
	}
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

bool UScWASC_Base::TryEvadeDamage(float& InOutAdjustedDamage, const FReceivedDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	bool bIsEvaded = false;

	if (HasMatchingGameplayTag(FScWGameplayTags::State_EvadeAnyDamage))
	{
		bIsEvaded = true;
	}
	else
	{
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
			bIsEvaded = true;
		}
	}
	if (bIsEvaded)
	{
		//AccumulateEvadedDamage(InOutAdjustedDamage);
		return true;
	}
	else
	{
		return false;
	}
}

bool UScWASC_Base::TryApplyDamage(float InDamage, const FReceivedDamageData& InData)
{
	check(IsOwnerActorAuthoritative());

	if (InDamage <= 0.0f)
	{
		return false;
	}
	LastAppliedDamage = InDamage;
	LastAppliedDamageData = InData;
	LastAppliedDamagePrevHealth = GetHealth();

	TSubclassOf<UGameplayEffect> ApplyDamageGameplayEffectClass = UScWGameplayFunctionLibrary::GetApplyDamageGameplayEffectClassForType(this, InData.DamageType);
	ensureReturn(ApplyDamageGameplayEffectClass, false);

	AccumulateAppliedDamage(LastAppliedDamage);

	FGameplayEffectContextHandle DamageEffectContext = MakeEffectContext();
	DamageEffectContext.AddSourceObject(this);
	DamageEffectContext.AddInstigator(InData.Instigator, InData.Source);

	FGameplayEffectSpecHandle DamageEffectHandle = MakeOutgoingSpec(ApplyDamageGameplayEffectClass, 1.0f, DamageEffectContext);
	DamageEffectHandle.Data->SetSetByCallerMagnitude(FScWGameplayTags::SetByCaller_Magnitude, -LastAppliedDamage);

	if (InData.DamageType)
	{
		DamageEffectHandle.Data->SetSetByCallerMagnitude(FScWGameplayTags::SetByCaller_Duration, InData.DamageType->ApplyDamageGameplayEffectDuration);
	}
	FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectSpecToSelf(*DamageEffectHandle.Data.Get());
	ensureReturn(EffectHandle.WasSuccessfullyApplied(), false);

	// Damage impact GameplayCue
	{
		FGameplayCueParameters ImpactCueParams = FGameplayCueParameters(MakeEffectContext());
		ImpactCueParams.Instigator = InData.Instigator ? InData.Instigator->GetPawn() : nullptr;
		ImpactCueParams.EffectCauser = InData.Source;
		ImpactCueParams.Location = InData.HitResult.ImpactPoint;
		ImpactCueParams.Normal = InData.HitResult.ImpactNormal;
		ImpactCueParams.RawMagnitude = InDamage;
		ImpactCueParams.NormalizedMagnitude = FMath::Min(InDamage / GetMaxHealth(), 1.0f);
		ExecuteGameplayCue(FScWGameplayTags::GameplayCue_Damage_Impact, ImpactCueParams);
	}
	return true;
}
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
	if (HasMatchingGameplayTag(FScWGameplayTags::Input_Block_Abilities))
	{
		return;
	}
	ensure(!PressedInputIDSet.Contains(InInputID));
	PressedInputIDSet.Add(InInputID);

	Super::AbilityLocalInputPressed(InInputID);
	OnInputPressedDelegate.Broadcast(InInputID);
}

void UScWASC_Base::AbilityLocalInputReleased(int32 InInputID) // UAbilitySystemComponent
{
	if (HasMatchingGameplayTag(FScWGameplayTags::Input_Block_Abilities))
	{
		return;
	}
	ensure(PressedInputIDSet.Contains(InInputID));
	PressedInputIDSet.Remove(InInputID);

	Super::AbilityLocalInputReleased(InInputID);
	OnInputReleasedDelegate.Broadcast(InInputID);
}
//~ End Input

//~ Begin Combo
void UScWASC_Base::QueueComboMove(const UScWComboMoveData* InComboMoveData)
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
		const UScWComboMoveData* AcceptedComboMove = QueuedComboMove;
		QueuedComboMove = nullptr;
		OnQueuedComboMoveAcceptedDelegate.Broadcast(AcceptedComboMove);

		AddComboMove(AcceptedComboMove, true, true);
	}
}

void UScWASC_Base::DenyQueuedComboMove()
{
	if (QueuedComboMove)
	{
		const UScWComboMoveData* DeniedComboMove = QueuedComboMove;
		QueuedComboMove = nullptr;
		OnQueuedComboMoveDeniedDelegate.Broadcast(DeniedComboMove);

	}
}

void UScWASC_Base::AddComboMove(const UScWComboMoveData* InComboMoveData, bool InUpdateRelevantCombo, bool InResetIfNoRelevantCombo)
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
