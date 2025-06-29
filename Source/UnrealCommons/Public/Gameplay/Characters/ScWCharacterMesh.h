// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "Gameplay/ScWASC_InitInterface.h"

#include "ScWCharacterMesh.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "[ScW] Character Mesh (Component)"))
class UNREALCOMMONS_API UScWCharacterMesh : public USkeletalMeshComponent, public IScWASC_InitInterface, public IScWCharacterData_InitInterface
{
	GENERATED_BODY()
	
public:	
	
	UScWCharacterMesh();
	
//~ Begin Initialize
public:
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) override; // IScWASC_InitInterface
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
//~ End Initialize

//~ Begin Damage
public:

	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite)
	FName RagdollCollisionProfileName;

protected:

	UFUNCTION()
	void HandleDamageApplied(float InDamage, const struct FReceivedDamageData& InData);

	UFUNCTION()
	void HandleDied();
//~ End Damage
};
