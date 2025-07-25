// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWASC_InitInterface.h"
#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "ScWCharacterMesh_Base.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "[ScW] Character Mesh Base (Component)"))
class UNREALCOMMONS_API UScWCharacterMesh_Base : public USkeletalMeshComponent, public IScWCharacterData_InitInterface, public IScWASC_InitInterface
{
	GENERATED_BODY()

public:

	UScWCharacterMesh_Base();

//~ Begin Initialize
protected:
	virtual void OnRegister() override; // UActorComponent
	virtual void BeginPlay() override; // UActorComponent
public:
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) override; // IScWASC_InitInterface
//~ End Initialize
	
//~ Begin Owner
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<class AScWCharacter> OwnerCharacter;
//~ End Owner

//~ Begin Updates
protected:

	UFUNCTION()
	void OnOwnerIsInFirstPersonViewChanged(const bool bInIsInFirstPersonView);

	UFUNCTION()
	void OnOwnerHandheldChanged(class AScWHandheld* InPrevHandheld, class AScWHandheld* InNewHandheld);

	virtual void UpdateFromIsInFirstPersonView(bool bInIsInFirstPersonView);
	virtual void UpdateFromHandheld(class AScWHandheld* InHandheld);
//~ End Updates
};
