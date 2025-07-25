// Scientific Ways

#include "Animations/ScWAnimInstance_FirstPersonLegs.h"

UScWAnimInstance_FirstPersonLegs::UScWAnimInstance_FirstPersonLegs()
{
	
}

//~ Begin Static
void UScWAnimInstance_FirstPersonLegs::SetIncapped(const bool InValue)
{
	bIncapped = InValue;
	BP_OnUpdateIncapped();
}
//~ End Static
