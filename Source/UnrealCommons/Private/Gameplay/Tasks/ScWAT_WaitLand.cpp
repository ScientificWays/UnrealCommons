// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitLand.h"

UScWAT_WaitLand* UScWAT_WaitLand::WaitLand(UGameplayAbility* InOwningAbility)
{
	UScWAT_WaitLand* OutTaskObject = NewAbilityTask<UScWAT_WaitLand>(InOwningAbility);
	return OutTaskObject;
}

void UScWAT_WaitLand::Activate()
{
	ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor());
	check(Character);

	LandedDelegate.BindUFunction(this, TEXT("OnPlayerLandedCallback"));
	Character->LandedDelegate.Add(LandedDelegate);

	Super::Activate();
}

void UScWAT_WaitLand::OnDestroy(bool bInAbilityIsEnding)
{
	ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor());
	check(Character);

	Character->LandedDelegate.Remove(LandedDelegate);
	Super::OnDestroy(bInAbilityIsEnding);
}

void UScWAT_WaitLand::OnPlayerLandedCallback(const FHitResult& InHit)
{
	OnPlayerLanded.Broadcast();
	EndTask();
}
