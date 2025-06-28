// Scientific Ways

#include "AI/ScWAIPatrolPoint.h"

AScWAIPatrolPoint::AScWAIPatrolPoint()
{
	
}

//~ Begin Location
FVector AScWAIPatrolPoint::BP_GetMoveToLocation_Implementation() const
{
	return GetActorLocation();
}
//~ End Location