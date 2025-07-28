// Scientific Ways

#include "Gameplay/Interact/ScWInteractComponent.h"

UScWInteractComponent::UScWInteractComponent()
{
	PrivateInteractData = FScWInteractData::Invalid;
}

//~ Begin Initialize
void UScWInteractComponent::OnRegister() // UActorComponent
{
	Super::OnRegister();

	
}

void UScWInteractComponent::BeginPlay() // UActorComponent
{
	Super::BeginPlay();

	
}
//~ End Initialize

//~ Begin State
bool UScWInteractComponent::BP_CanReceiveInteractFrom_Implementation(UScWInteractComponent* InSource, float& OutInteractDuration) const
{
	OutInteractDuration = 1.0f;
	return true;
}
//~ End State

//~ Begin Trace
FVector UScWInteractComponent::BP_GetInteractTraceStartLocation_Implementation() const
{
	ensureReturn(GetOwner(), FAISystem::InvalidLocation);
	return GetOwner()->GetActorLocation();
}

FVector UScWInteractComponent::BP_GetInteractTraceDirection_Implementation() const
{
	ensureReturn(GetOwner(), FVector::ForwardVector);
	return GetOwner()->GetActorRotation().Vector();
}

float UScWInteractComponent::BP_GetInteractTraceMaxDistance_Implementation() const
{
	return 256.0f;
}

float UScWInteractComponent::BP_GetInteractTraceWidth_Implementation() const
{
	return 2.56f;
}

float UScWInteractComponent::BP_GetInteractTraceCheckRadius_Implementation() const
{
	return 25.6f;
}
//~ End Trace
