// G.o.o.P Studios

#include "GAS/ATAAS_Character.h"

UATAAS_Character::UATAAS_Character()
{
	InitMoveSpeedMul(1.0f);
	InitMoveSpeedBase(600.0f);
	InitMoveSpeedCrouchedBase(300.0f);
	InitMoveSpeedMax(1000.0f);

	InitMaxAcceleration(1500.0f);
	InitBrakingDecelerationWalking(2000.0f);
	InitGravityScale(1.0f);
	InitAirControl(0.1f);
}

//~ Begin Replication
void UATAAS_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // UObject
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MaxWalkSpeed);
	DOREPLIFETIME(ThisClass, MaxWalkSpeedCrouched);
	DOREPLIFETIME(ThisClass, MaxAcceleration);
	DOREPLIFETIME(ThisClass, BrakingDecelerationWalking);
	DOREPLIFETIME(ThisClass, GravityScale);
	DOREPLIFETIME(ThisClass, AirControl);
	DOREPLIFETIME(ThisClass, Stamina);
	DOREPLIFETIME(ThisClass, MaxStamina);
}
//~ End Replication

//~ Begin Change
void UATAAS_Character::PreAttributeChange(const FGameplayAttribute& InAttribute, float& InOutNewValue) // UAttributeSet
{
	if (InAttribute == GetMaxWalkSpeedAttribute() || InAttribute == GetMaxWalkSpeedCrouchedAttribute())
	{
		InOutNewValue = FMath::Clamp(InOutNewValue, 0.0f, GetMoveSpeedMax());
	}
	else if (InAttribute == GetStaminaAttribute())
	{
		InOutNewValue = FMath::Clamp(InOutNewValue, 0.0f, GetMaxStamina());
	}
	else
	{
		Super::PreAttributeChange(InAttribute, InOutNewValue); // Default is InOutNewValue = FMath::Max(InOutNewValue, 0.0f)
	}
}

void UATAAS_Character::PostAttributeChange(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue) // UAttributeSet
{
	Super::PostAttributeChange(InAttribute, InPrevValue, InNewValue);

	if (InAttribute == GetHealthAttribute())
	{
		
	}
	else if (InAttribute == GetMaxHealthAttribute())
	{
		
	}
	else if (InAttribute == GetMoveSpeedMulAttribute())
	{
		if (GetOwningActor()->HasAuthority())
		{
			SetMaxWalkSpeed(GetMoveSpeedBase() * InNewValue);
			SetMaxWalkSpeedCrouched(GetMoveSpeedCrouchedBase() * InNewValue);
		}
	}
	else if (InAttribute == GetMoveSpeedMaxAttribute())
	{
		if (GetOwningActor()->HasAuthority())
		{
			SetMaxWalkSpeed(GetMoveSpeedBase() * GetMoveSpeedMul());
			SetMaxWalkSpeedCrouched(GetMoveSpeedCrouchedBase() * GetMoveSpeedMul());
		}
	}
	else if (InAttribute == GetMoveSpeedBaseAttribute())
	{
		if (GetOwningActor()->HasAuthority())
		{
			SetMaxWalkSpeed(InNewValue * GetMoveSpeedMul());
		}
	}
	else if (InAttribute == GetMoveSpeedCrouchedBaseAttribute())
	{
		if (GetOwningActor()->HasAuthority())
		{
			SetMaxWalkSpeedCrouched(InNewValue * GetMoveSpeedMul());
		}
	}
}
//~ End Change

//~ Begin Replication
void UATAAS_Character::OnRep_Stamina(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Stamina, InPrevValue);
}

void UATAAS_Character::OnRep_MaxStamina(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxStamina, InPrevValue);
}

void UATAAS_Character::OnRep_MaxWalkSpeed(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxWalkSpeed, InPrevValue);
}

void UATAAS_Character::OnRep_MaxWalkSpeedCrouched(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxWalkSpeedCrouched, InPrevValue);
}

void UATAAS_Character::OnRep_MaxAcceleration(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxAcceleration, InPrevValue);
}

void UATAAS_Character::OnRep_BrakingDecelerationWalking(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BrakingDecelerationWalking, InPrevValue);
}

void UATAAS_Character::OnRep_GravityScale(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, GravityScale, InPrevValue);
}

void UATAAS_Character::OnRep_AirControl(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AirControl, InPrevValue);
}
//~ End Replication