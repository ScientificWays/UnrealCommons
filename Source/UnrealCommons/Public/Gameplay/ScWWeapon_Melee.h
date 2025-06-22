// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWWeapon_Base.h"

#include "ScWWeapon_Melee.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "[ScW] Weapon Melee"))
class UNREALCOMMONS_API AScWWeapon_Melee : public AScWWeapon_Base
{
	GENERATED_BODY()

public:

	AScWWeapon_Melee(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Initialize
public:
	virtual void BP_UpdateFromDataAsset_Implementation() override; // AScWWeapon_Base
protected:
	virtual void BeginPlay() override; // AActor
//~ End Initialize
	
//~ Begin Components
public:

	UFUNCTION(Category = "Components", BlueprintCallable)
	UCapsuleComponent* GetCollision() const { return Collision; }

protected:

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> Collision;
//~ End Components
};
