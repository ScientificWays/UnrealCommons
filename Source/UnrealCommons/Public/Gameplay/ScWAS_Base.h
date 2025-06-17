// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "ScWAS_Base.generated.h"

#define ATTRIBUTE_ACCESSORS(InClass, InProperty)							\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(InClass, InProperty)					\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(InProperty)								\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(InProperty)								\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(InProperty)

/**
 * 
 */
UCLASS(meta = (DisplayName = "[ScW] AS Base"))
class UNREALCOMMONS_API UScWAS_Base : public UAttributeSet
{
	GENERATED_BODY()

public:

	UScWAS_Base();

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

	UPROPERTY(ReplicatedUsing = "OnRep_Health", BlueprintReadOnly)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ThisClass, Health)

	UPROPERTY(ReplicatedUsing = "OnRep_MaxHealth", BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth)
//~ End Attributes

//~ Begin Replication
protected:

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& InPrevValue);
//~ End Replication
};
