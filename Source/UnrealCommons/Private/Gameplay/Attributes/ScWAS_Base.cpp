// Scientific Ways

#include "Gameplay/Attributes/ScWAS_Base.h"

UScWAS_Base::UScWAS_Base()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

//~ Begin Replication
void UScWAS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // UObject
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Health);
	DOREPLIFETIME(ThisClass, MaxHealth);
}
//~ End Replication

//~ Begin Change
void UScWAS_Base::PreAttributeChange(const FGameplayAttribute& InAttribute, float& InOutNewValue) // UAttributeSet
{
	if (InAttribute == GetHealthAttribute())
	{
		InOutNewValue = FMath::Clamp(InOutNewValue, 0.0f, GetMaxHealth());
	}
	else
	{
		InOutNewValue = FMath::Max(InOutNewValue, 0.0f);
	}
}

void UScWAS_Base::PostAttributeChange(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue) // UAttributeSet
{
	Super::PostAttributeChange(InAttribute, InPrevValue, InNewValue);

	if (InAttribute == GetMaxHealthAttribute())
	{
		if (InPrevValue == 0.0f || InNewValue == 0.0f || GetHealth() == 0.0f)
		{
			SetHealth(InNewValue);
		}
		else
		{
			SetHealth(GetHealth() * InNewValue / InPrevValue);
		}
	}
}
//~ End Change

//~ Begin Replication
void UScWAS_Base::OnRep_Health(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, InPrevValue);
}


void UScWAS_Base::OnRep_MaxHealth(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, InPrevValue);
}
//~ End Replication