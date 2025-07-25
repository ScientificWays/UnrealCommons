// Scientific Ways

#include "Gameplay/Tasks/ScWAT_WaitOverlap.h"

//~ Begin Initialize
UScWAT_WaitOverlap* UScWAT_WaitOverlap::IDWaitOverlap(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, UClass* InOverlapClassFilter)
{
	UScWAT_WaitOverlap* OutTaskObject = NewAbilityTask<UScWAT_WaitOverlap>(InOwningAbility);
	OutTaskObject->CheckComponent = InCheckComponent;
	OutTaskObject->OverlapClassFilter = InOverlapClassFilter;
	return OutTaskObject;
}

void UScWAT_WaitOverlap::Activate()
{
	ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor());

	check(Character);

	OverlapDelegate.BindUFunction(this, TEXT("OnOverlapCallback"));

	Character->GetCapsuleComponent()->OnComponentBeginOverlap.Add(OverlapDelegate);

	Super::Activate();
}

void UScWAT_WaitOverlap::OnDestroy(bool bInAbilityIsEnding)
{
	CheckComponent->OnComponentBeginOverlap.Remove(OverlapDelegate);

	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Task
void UScWAT_WaitOverlap::OnOverlapCallback(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	if (InOtherActor->IsA(OverlapClassFilter))
	{
		OnOverlap.Broadcast(InOtherActor);

		EndTask();
	}
}
//~ End Task
