// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "Gameplay/ScWAS_Base.h"

#include "ScWAS_Character.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AS Character"))
class UNREALCOMMONS_API UScWAS_Character : public UScWAS_Base
{
	GENERATED_BODY()

public:

	UScWAS_Character();

//~ Begin Replication
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // UObject
//~ End Replication

//~ Begin Change
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& InAttribute, float& InOutNewValue) override; // UAttributeSet
	virtual void PostAttributeChange(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue) override; // UAttributeSet
//~ End Change

//~ Begin Attributes
public:

	UPROPERTY(ReplicatedUsing = "OnRep_Stamina", BlueprintReadOnly)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(ThisClass, Stamina)

	UPROPERTY(ReplicatedUsing = "OnRep_MaxStamina", BlueprintReadOnly)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxStamina)

	UPROPERTY(ReplicatedUsing = "OnRep_MaxWalkSpeed", BlueprintReadOnly)
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxWalkSpeed)

	UPROPERTY(ReplicatedUsing = "OnRep_MaxWalkSpeedCrouched", BlueprintReadOnly)
	FGameplayAttributeData MaxWalkSpeedCrouched;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxWalkSpeedCrouched)

	UPROPERTY(ReplicatedUsing = "OnRep_MaxAcceleration", BlueprintReadOnly)
	FGameplayAttributeData MaxAcceleration;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxAcceleration)

	UPROPERTY(ReplicatedUsing = "OnRep_BrakingDecelerationWalking", BlueprintReadOnly)
	FGameplayAttributeData BrakingDecelerationWalking;
	ATTRIBUTE_ACCESSORS(ThisClass, BrakingDecelerationWalking)

	/* Server-side */
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeedMul;
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeedMul)

	/* Server-side */
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeedMax;
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeedMax)

	/* Server-side */
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeedBase;
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeedBase)

	/* Server-side */
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeedCrouchedBase;
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeedCrouchedBase)

	UPROPERTY(ReplicatedUsing = "OnRep_GravityScale", BlueprintReadOnly)
	FGameplayAttributeData GravityScale;
	ATTRIBUTE_ACCESSORS(ThisClass, GravityScale)

	UPROPERTY(ReplicatedUsing = "OnRep_AirControl", BlueprintReadOnly)
	FGameplayAttributeData AirControl;
	ATTRIBUTE_ACCESSORS(ThisClass, AirControl)
//~ End Attributes

//~ Begin Replication
protected:

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_MaxWalkSpeed(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_MaxWalkSpeedCrouched(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_MaxAcceleration(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_BrakingDecelerationWalking(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_GravityScale(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_AirControl(const FGameplayAttributeData& InPrevValue);
//~ End Replication
};
