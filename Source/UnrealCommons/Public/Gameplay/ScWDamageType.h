// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWDamageType.generated.h"

/**
 *
 */
UCLASS(Const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Damage Type"))
class UNREALCOMMONS_API UScWDamageType : public UDamageType
{
	GENERATED_BODY()

public:	

	UScWDamageType();

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText();

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSoftObjectPtr<UTexture2D> Image = nullptr;
//~ End UI

//~ Begin GAS
public:

	UPROPERTY(Category = "GAS", EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Game"))
	TSubclassOf<UGameplayEffect> ApplyDamageGameplayEffectClass = nullptr;
//~ End GAS

//~ Begin Impact
public:
	void ApplyImpactImpulseToCharacterMovement(class UScWCharacterMovement* InCharacterMovement, const FHitResult& InHitResult) const;
	void ApplyImpactImpulseToPhysicsBody(UPrimitiveComponent* InBody, const FHitResult& InHitResult) const;

	UPROPERTY(Category = "Impact", EditDefaultsOnly, BlueprintReadOnly)
	bool bApplyImpactImpulse = true;

protected:

	UFUNCTION(Category = "Impact", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Impact Impulse for Hit"))
	FVector BP_GetImpactImpulseForHit(const FHitResult& InHitResult) const;

//~ End Impact
};
