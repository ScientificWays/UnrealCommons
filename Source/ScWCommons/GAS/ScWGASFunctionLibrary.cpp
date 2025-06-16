// G.o.o.P Studios

#include "GASFunctionLibrary.h"

#include "GAS/ATADT_Base.h"

#include "World/ATAGameState_Base.h"

//~ Begin Health
TSubclassOf<UGameplayEffect> UGASFunctionLibrary::GetSetHealthGameplayEffectClass(const UObject* InWCO)
{
	const AATAGameState_Base* BaseGameState = AATAGameState_Base::TryGetAtaGameState(InWCO);
	if (BaseGameState && BaseGameState->DefaultSetHealthGameplayEffectClass)
	{
		return BaseGameState->DefaultSetHealthGameplayEffectClass;
	}
	UE_LOG(LogAtaGameplay, Error, TEXT("UGASFunctionLibrary::GetDamageGameplayEffectClassForType() GameState does not have valid DefaultSetHealthGameplayEffectClass!"));
	return nullptr;
}

TSubclassOf<UGameplayEffect> UGASFunctionLibrary::GetAddHealthGameplayEffectClass(const UObject* InWCO)
{
	const AATAGameState_Base* BaseGameState = AATAGameState_Base::TryGetAtaGameState(InWCO);
	if (BaseGameState && BaseGameState->DefaultAddHealthGameplayEffectClass)
	{
		return BaseGameState->DefaultAddHealthGameplayEffectClass;
	}
	UE_LOG(LogAtaGameplay, Error, TEXT("UGASFunctionLibrary::GetDamageGameplayEffectClassForType() GameState does not have valid DefaultSetHealthGameplayEffectClass!"));
	return nullptr;
}
//~ End Health

//~ Begin Damage
TSubclassOf<UGameplayEffect> UGASFunctionLibrary::GetApplyDamageGameplayEffectClassForType(const UObject* InWCO, const UDamageType* InDamageType)
{
	const UATADT_Base* ATADamageType = Cast<UATADT_Base>(InDamageType);
	if (ATADamageType && ATADamageType->ApplyDamageGameplayEffectClass)
	{
		return ATADamageType->ApplyDamageGameplayEffectClass;
	}
	const AATAGameState_Base* BaseGameState = AATAGameState_Base::TryGetAtaGameState(InWCO);
	if (BaseGameState && BaseGameState->DefaultApplyDamageGameplayEffectClass)
	{
		return BaseGameState->DefaultApplyDamageGameplayEffectClass;
	}
	UE_LOG(LogAtaGameplay, Error, TEXT("UGASFunctionLibrary::GetApplyDamageGameplayEffectClassForType() Neither GameState nor InDamageType (%s) have valid ApplyDamageGameplayEffectClass!"), *InDamageType->GetName());
	return nullptr;
}

float UGASFunctionLibrary::ApplyPointDamage(AActor* InSourceActor, AActor* InTargetActor, float InDamage, const FVector& InHitFromDirection, const FHitResult& InHitResult, AController* InInstigator, TSubclassOf<UDamageType> InDamageTypeClass)
{
	if (InTargetActor == nullptr)
	{
		return 0.0f;
	}
	if (InDamage <= 0.0f)
	{
		return 0.0f;
	}
	const TSubclassOf<UDamageType> ValidDamageTypeClass = InDamageTypeClass ? InDamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FPointDamageEvent PointDamageEvent = { InDamage, InHitResult, InHitFromDirection, ValidDamageTypeClass };

	return InTargetActor->TakeDamage(InDamage, PointDamageEvent, InInstigator, InSourceActor);
}

/*void UGASFunctionLibrary::HandleFirearmBulletHit(UIDASC_Base* InShootingASC, const FHitResult& InHitResult, const UFirearmDataAsset* InFirearmAsset)
{
	if (InShootingASC == nullptr)
	{
		UE_LOG(LogServer, Error, TEXT("UGASFunctionLibrary::HandleFirearmBulletHit() InShootingASC is nullptr!"));
		return;
	}
	check(InShootingASC->IsOwnerActorAuthoritative());

	AActor* HitActor = InHitResult.GetActor();
	if (HitActor && HitActor->CanBeDamaged())
	{
		float DamageFalloff = InFirearmAsset->ShootDamageFalloffCurve->GetFloatValue(InHitResult.Distance / InFirearmAsset->ShootMaxRange);
		float AdjustedDamage = InFirearmAsset->ShootImpactDamage * DamageFalloff;

		if (AdjustedDamage <= 0.0f)
		{
			return;
		}
		APawn* ShootingPawn = InShootingASC->GetOwnerPawn();
		AController* ShootingController = InShootingASC->GetOwnerController();

		FVector Direction = ShootingPawn->GetActorLocation() - HitActor->GetActorLocation();
		Direction.Normalize();

		if (UIDASC_Base* TargetASC = UIDAbilitySystemGlobals::GetBaseASCFromActor(HitActor))
		{
			if (TargetASC->ReceiveDamage(AdjustedDamage, { Direction, InHitResult, InFirearmAsset->ShootImpactDamageType.Get(), ShootingController, ShootingPawn })
				&& InFirearmAsset->ShootTargetEffectClass.Get())
			{
				TargetASC->TryApplyGameplayEffectByClass(InFirearmAsset->ShootTargetEffectClass.Get());
			}
			else
			{
				return;
			}
		}
		else
		{
			ApplyPointDamage(ShootingPawn, HitActor, AdjustedDamage, Direction, InHitResult, ShootingController, InFirearmAsset->ShootImpactDamageType.Get());
		}
		TryApplyHitImpulse(InHitResult, InFirearmAsset->ShootImpactImpulseScale);
	}
	//UAISense_Hearing::ReportNoiseEvent(InShootingASC, InHitResult.ImpactPoint, 1.0f, InShootingASC->GetOwnerPawn(), InFirearmAsset->BulletData.HitMaxHearingRange);
}

void UGASFunctionLibrary::HandleMeleeSwingHit(UIDASC_Character* InSwingingASC, const FHitResult& InHitResult, const UMeleeDataAsset* InMeleeAsset)
{
	if (InSwingingASC == nullptr)
	{
		UE_LOG(LogServer, Error, TEXT("UGASFunctionLibrary::HandleMeleeSwingHit() InSwingingASC is nullptr!"));
		return;
	}
	if (InMeleeAsset == nullptr)
	{
		UE_LOG(LogServer, Error, TEXT("UGASFunctionLibrary::HandleMeleeSwingHit() InMeleeAsset is nullptr!"));
		return;
	}
	check(InSwingingASC->IsOwnerActorAuthoritative());

	AActor* HitActor = InHitResult.GetActor();
	if (HitActor && HitActor->CanBeDamaged())
	{
		const UIDAttributeSet_Character* AttributeSet = InSwingingASC->GetAttributeSet<UIDAttributeSet_Character>();
		check(AttributeSet);

		float AdjustedDamage = InSwingingASC->GetMeleeSwingDamage(InMeleeAsset, InHitResult);

		AIDCharacter_Base* SwingingCharacter = InSwingingASC->GetOwnerCharacter();
		AController* SwingingController = InSwingingASC->GetOwnerController();

		FVector Direction = SwingingCharacter->GetActorLocation() - HitActor->GetActorLocation();
		Direction.Normalize();

		if (UIDASC_Base* TargetASC = UIDAbilitySystemGlobals::GetBaseASCFromActor(HitActor))
		{
			if (TargetASC->ReceiveDamage(AdjustedDamage, { Direction, InHitResult, InMeleeAsset->SwingImpactDamageType.Get(), SwingingController, SwingingCharacter }))
			{
				const bool bRiposting = InSwingingASC->HasMatchingGameplayTag(FIDGameplayTags::State_Riposting);
				if (bRiposting && InMeleeAsset->BlockData.RiposteTargetEffectClass.Get())
				{
					TargetASC->TryApplyGameplayEffectByClass(InMeleeAsset->BlockData.RiposteTargetEffectClass.Get());
				}
				if (!bRiposting || InMeleeAsset->bApplySwingTargetEffectOnRiposte)
				{
					TargetASC->TryApplyGameplayEffectByClass(InMeleeAsset->SwingTargetEffectClass.Get());
				}
			}
			else
			{
				return;
			}
		}
		else
		{
			ApplyPointDamage(SwingingCharacter, HitActor, AdjustedDamage, Direction, InHitResult, SwingingController, InMeleeAsset->SwingImpactDamageType.Get());
		}
		TryApplyHitImpulse(InHitResult, InMeleeAsset->SwingImpactImpulseScale);
	}
	//UAISense_Hearing::ReportNoiseEvent(InSwingingASC, InHitResult.ImpactPoint, 1.0f, InSwingingASC->GetOwnerPawn(), InMeleeAsset->SwingHitMaxHearingRange);
}*/
//~ End Damage

//~ Begin GameplayTags
FDelegateHandle_BlueprintWrapper UGASFunctionLibrary::RegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FGameplayTagNumChangedEvent InEvent, FGameplayTag InTag, EGameplayTagEventType::Type InType)
{
	return { InASC->RegisterGameplayTagEvent(InTag, InType).AddUFunction(InEvent.GetUObject(), InEvent.GetFunctionName()) };
}

void UGASFunctionLibrary::UnRegisterGameplayTagEvent(UAbilitySystemComponent* InASC, FDelegateHandle_BlueprintWrapper InHandleWrapper, FGameplayTag InTag, EGameplayTagEventType::Type InType)
{
	InASC->RegisterGameplayTagEvent(InTag, InType).Remove(InHandleWrapper.Handle);
}
//~ End GameplayTags
