// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/Characters/ScWCharacterMesh_Base.h"

#include "ScWCharacterMesh_FirstPerson.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = ("Character"), meta = (DisplayName = "[ScW] FirstPerson Character Mesh (Component)", BlueprintSpawnableComponent))
class UNREALCOMMONS_API UScWCharacterMesh_FirstPerson : public UScWCharacterMesh_Base
{
	GENERATED_BODY()

public:

	UScWCharacterMesh_FirstPerson();

//~ Begin Initialize
protected:
	virtual void BeginPlay() override; // UActorComponent
	virtual void UpdateSwaySpringArmComponent();
public:
	virtual void InitFromCharacterData(const class UScWCharacterData* InInitCharacterData) override; // IScWCharacterData_InitInterface
	virtual void InitFromASC(class UScWASC_Base* InInitASC, AActor* InOwnerActor, AActor* InAvatarActor) override; // IScWASC_InitInterface
//~ End Initialize
	
//~ Begin Updates
protected:
	virtual void UpdateFromIsInFirstPersonView(bool bInIsInFirstPersonView) override; // UScWCharacterMesh_Base
	virtual void UpdateFromHandheld(class AScWHandheld* InHandheld) override; // UScWCharacterMesh_Base
//~ End Updates

//~ Begin Camera
public:

	//UPROPERTY(Category = FOV, EditAnywhere, BlueprintReadWrite)
	//float OverrideFOV;

protected:
	//virtual FMatrix GetRenderMatrix() const override; // UPrimitiveComponent

	UPROPERTY(Transient)
	TObjectPtr<APlayerCameraManager> PlayerCameraManager;
//~ End Camera

//~ Begin Sway
protected:
	virtual void OnAimingTagChanged(const FGameplayTag InCallbackTag, int32 InNewCount);

	UPROPERTY(Category = "Sway", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SwaySpringArmComponent;

	UPROPERTY(Category = "Sway", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SwaySpringArmComponent != nullptr", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float DefaultSwaySpeed;

	UPROPERTY(Category = "Sway", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SwaySpringArmComponent != nullptr", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float DefaultSwayRotationSpeed;

	UPROPERTY(Category = "Sway", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SwaySpringArmComponent != nullptr", ClampMin = "0.0", UIMin = "0.0"))
	float DefaultSwayMaxDistance;

	UPROPERTY(Category = "Sway", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SwaySpringArmComponent != nullptr", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float AimingSwaySpeed;

	UPROPERTY(Category = "Sway", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SwaySpringArmComponent != nullptr", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float AimingSwayRotationSpeed;

	UPROPERTY(Category = "Sway", EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SwaySpringArmComponent != nullptr", ClampMin = "0.0", UIMin = "0.0"))
	float AimingSwayMaxDistance;
//~ End Sway
};
