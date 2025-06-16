// G.o.o.P Studios

#pragma once

#include "Ata_DreamComeTrue.h"

#include "GAS/ATAWeapon_Base.h"

#include "ATAWeapon_Melee.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class AATAWeapon_Melee : public AATAWeapon_Base
{
	GENERATED_BODY()

public:

	AATAWeapon_Melee(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Initialize
public:
	virtual void K2_UpdateFromDataAsset_Implementation() override; // AATAWeapon_Base
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
