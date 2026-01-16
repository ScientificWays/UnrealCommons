// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Characters/ScWCharacterMesh_Base.h"

#include "ScWCharacterMesh_ThirdPerson.generated.h"

/**
 *
 */
UCLASS(ClassGroup = ("Character"), meta = (DisplayName = "[ScW] ThirdPerson Character Mesh (Component)", BlueprintSpawnableComponent))
class UNREALCOMMONS_API UScWCharacterMesh_ThirdPerson : public UScWCharacterMesh_Base
{
	GENERATED_BODY()
	
public:	
	
	UScWCharacterMesh_ThirdPerson();
	
//~ Begin Initialize
protected:
	virtual void BeginPlay() override; // UActorComponent
public:
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) override; // IScWASC_InitInterface
//~ End Initialize
	
//~ Begin Updates
protected:
	virtual void UpdateFromIsInFirstPersonView(bool bInIsInFirstPersonView) override; // UScWCharacterMesh_Base
	virtual void UpdateFromHandheld(class AScWHandheld* InHandheld) override; // UScWCharacterMesh_Base
//~ End Updates
	
//~ Begin Attributes
public:

protected:
	void OnMassAttributeChanged(const FOnAttributeChangeData& InData);

	FDelegateHandle MassChangedDelegateHandle;
//~ End Attributes

//~ Begin Damage
public:

	UFUNCTION(Category = "Damage", BlueprintNativeEvent, BlueprintCallable, BlueprintCosmetic, meta = (DisplayName = "Activate Ragdoll"))
	void BP_ActivateRagdoll();

	UPROPERTY(Category = "Damage", EditAnywhere, BlueprintReadWrite)
	FName RagdollCollisionProfileName;

protected:

	UFUNCTION()
	void HandleDamageApplied(float InDamage, const struct FReceivedDamageData& InData);

	UFUNCTION()
	void HandleDied();
//~ End Damage
};
