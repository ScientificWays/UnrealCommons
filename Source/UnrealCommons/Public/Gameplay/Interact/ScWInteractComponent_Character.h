// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "Gameplay/Interact/ScWInteractComponent.h"

#include "ScWInteractComponent_Character.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Interact"), meta = (DisplayName = "[ScW] Interact Component (Character)", BlueprintSpawnableComponent))
class UNREALCOMMONS_API UScWInteractComponent_Character : public UScWInteractComponent, public IScWCharacterData_InitInterface
{
	GENERATED_BODY()

public:

	UScWInteractComponent_Character();

//~ Begin Initialize
protected:
	virtual void OnRegister() override; // UActorComponent
	virtual void BeginPlay() override; // UActorComponent
public:
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
//~ End Initialize

//~ Begin State
protected:
	virtual bool BP_CanReceiveInteractFrom_Implementation(UScWInteractComponent* FromInteractComponent, float& OutInteractDuration) const override; // UScWInteractComponent
//~ End State
	
//~ Begin Trace
protected:
	virtual FVector BP_GetInteractTraceStartLocation_Implementation() const override; // UScWInteractComponent
	virtual FVector BP_GetInteractTraceDirection_Implementation() const override; // UScWInteractComponent
	virtual float BP_GetInteractTraceMaxDistance_Implementation() const override; // UScWInteractComponent
	virtual float BP_GetInteractTraceWidth_Implementation() const override; // UScWInteractComponent
	virtual float BP_GetInteractTraceCheckRadius_Implementation() const override; // UScWInteractComponent
//~ End Trace

//~ Begin Owner
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<class AScWCharacter> OwnerCharacter;
//~ End Owner
};
