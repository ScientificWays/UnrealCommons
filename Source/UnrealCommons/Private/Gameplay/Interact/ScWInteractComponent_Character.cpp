// Scientific Ways

#include "Gameplay/Interact/ScWInteractComponent_Character.h"

UScWInteractComponent_Character::UScWInteractComponent_Character()
{
	
}

//~ Begin Initialize
void UScWInteractComponent_Character::OnRegister() // UActorComponent
{
	Super::OnRegister();

	OwnerCharacter = GetOwner<AScWCharacter>();
	ensureReturn(OwnerCharacter);
}

void UScWInteractComponent_Character::BeginPlay() // UActorComponent
{
	Super::BeginPlay();

	
}

void UScWInteractComponent_Character::InitFromCharacterData(const UScWCharacterData* InInitCharacterData) // IScWCharacterData_InitInterface
{
	ensureReturn(InInitCharacterData);


}
//~ End Initialize

//~ Begin State
bool UScWInteractComponent_Character::BP_CanReceiveInteractFrom_Implementation(UScWInteractComponent* InSource, float& OutInteractDuration) const
{
	OutInteractDuration = 0.0f;
	return false;
}
//~ End State

//~ Begin Trace
FVector UScWInteractComponent_Character::BP_GetInteractTraceStartLocation_Implementation() const
{
	ensureReturn(OwnerCharacter, Super::BP_GetInteractTraceStartLocation_Implementation());
	return OwnerCharacter->GetPawnViewLocation();
}

FVector UScWInteractComponent_Character::BP_GetInteractTraceDirection_Implementation() const
{
	ensureReturn(OwnerCharacter, Super::BP_GetInteractTraceDirection_Implementation());
	return OwnerCharacter->GetBaseAimRotation().Vector();
}

float UScWInteractComponent_Character::BP_GetInteractTraceMaxDistance_Implementation() const
{
	ensureReturn(OwnerCharacter, Super::BP_GetInteractTraceMaxDistance_Implementation());
	return 256.0f;
}

float UScWInteractComponent_Character::BP_GetInteractTraceWidth_Implementation() const
{
	ensureReturn(OwnerCharacter, Super::BP_GetInteractTraceWidth_Implementation());
	return 2.56f;
}

float UScWInteractComponent_Character::BP_GetInteractTraceCheckRadius_Implementation() const
{
	ensureReturn(OwnerCharacter, Super::BP_GetInteractTraceCheckRadius_Implementation());
	return 25.6f;
}
//~ End Trace
