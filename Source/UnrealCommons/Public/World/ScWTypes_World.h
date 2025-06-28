// Scientific Ways

#pragma once

#include "UnrealCommons.h"

//#include "ScWTypes_World.generated.h"

#define TraceTypeQuery_Visibility				ETraceTypeQuery::TraceTypeQuery1
#define TraceTypeQuery_Camera					ETraceTypeQuery::TraceTypeQuery2
#define TraceTypeQuery_Melee					ETraceTypeQuery::TraceTypeQuery3

#define CollisionChannel_Visibility				ECollisionChannel::ECC_GameTraceChannel1
#define CollisionChannel_Camera					ECollisionChannel::ECC_GameTraceChannel2
#define CollisionChannel_Melee					ECollisionChannel::ECC_GameTraceChannel3

//#define ECC_Shoot								ECollisionChannel::ECC_GameTraceChannel1
//#define ECC_Interact							ECollisionChannel::ECC_GameTraceChannel2
//#define ECC_OnlyCharacterIgnore					ECollisionChannel::ECC_GameTraceChannel5
//#define ECC_OverlapAllShapes					ECollisionChannel::ECC_GameTraceChannel10

#define ObjectTypeQuery_WorldStatic				EObjectTypeQuery::ObjectTypeQuery1
#define ObjectTypeQuery_WorldDynamic			EObjectTypeQuery::ObjectTypeQuery2
#define ObjectTypeQuery_Pawn					EObjectTypeQuery::ObjectTypeQuery3
#define ObjectTypeQuery_PhysicsBody				EObjectTypeQuery::ObjectTypeQuery4
#define ObjectTypeQuery_Vehicle					EObjectTypeQuery::ObjectTypeQuery5
#define ObjectTypeQuery_Destructible			EObjectTypeQuery::ObjectTypeQuery6
//#define ObjectTypeQuery_PawnA					EObjectTypeQuery::ObjectTypeQuery7
//#define ObjectTypeQuery_PawnB					EObjectTypeQuery::ObjectTypeQuery8
//#define ObjectTypeQuery_Building				EObjectTypeQuery::ObjectTypeQuery9
//#define ObjectTypeQuery_Driver					EObjectTypeQuery::ObjectTypeQuery10
//#define ObjectTypeQuery_Volume					EObjectTypeQuery::ObjectTypeQuery11
