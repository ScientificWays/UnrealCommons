// G.o.o.P Studios

#include "GAS/ATAAS_Base.h"

UATAAS_Base::UATAAS_Base()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
}

//~ Begin Replication
void UATAAS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // UObject
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Health);
	DOREPLIFETIME(ThisClass, MaxHealth);
}
//~ End Replication

//~ Begin Change
void UATAAS_Base::PreAttributeChange(const FGameplayAttribute& InAttribute, float& InOutNewValue) // UAttributeSet
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

void UATAAS_Base::PostAttributeChange(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue) // UAttributeSet
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
void UATAAS_Base::OnRep_Health(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, InPrevValue);
}


void UATAAS_Base::OnRep_MaxHealth(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, InPrevValue);
}
//~ End Replication