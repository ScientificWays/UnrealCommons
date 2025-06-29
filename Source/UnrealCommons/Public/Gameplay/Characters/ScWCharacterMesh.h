// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWASC_InitInterface.h"
#include "Gameplay/Characters/ScWCharacterData_InitInterface.h"

#include "ScWCharacterMesh.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "[ScW] Character Mesh (Component)"))
class UNREALCOMMONS_API UScWCharacterMesh : public USkeletalMeshComponent, public IScWCharacterData_InitInterface, public IScWASC_InitInterface
{
	GENERATED_BODY()
	
public:	
	
	UScWCharacterMesh();
	
//~ Begin Initialize
public:
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) override; // IScWASC_InitInterface
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
