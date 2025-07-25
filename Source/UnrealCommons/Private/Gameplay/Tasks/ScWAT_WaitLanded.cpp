// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitLanded.h"

//~ Begin Initialize
UScWAT_WaitLanded* UScWAT_WaitLanded::WaitLanded(UGameplayAbility* InOwningAbility, ACharacter* InTargetCharacter)
{
	UScWAT_WaitLanded* OutTaskObject = NewAbilityTask<UScWAT_WaitLanded>(InOwningAbility);
	OutTaskObject->TargetCharacter = InTargetCharacter;
	return OutTaskObject;
}

void UScWAT_WaitLanded::Activate()
{
	ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor());
	check(Character);

	LandedScriptDelegate.BindUFunction(this, TEXT("OnPlayerLandedCallback"));
	Character->LandedDelegate.Add(LandedScriptDelegate);

	Super::Activate();
}

void UScWAT_WaitLanded::OnDestroy(bool bInAbilityIsEnding)
{
	ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor());
	check(Character);

	Character->LandedDelegate.Remove(LandedScriptDelegate);
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Task
void UScWAT_WaitLanded::OnPlayerLandedCallback(const FHitResult& InHitResult)
{
	OnPlayerLanded.Broadcast(InHitResult);
	EndTask();
}
//~ End Task
