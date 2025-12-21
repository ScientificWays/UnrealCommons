// Scientific Ways

#pragma once

#include "UnrealCommons.h"

#include "NativeGameplayTags.h"

namespace FScWGameplayTags
{
	// Abilities
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Flashlight);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_Crouch);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Movement_WalkSlow);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Handheld_Swing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interact);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_Stunned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_JumpActivate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_SprintActivate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_CrouchActivate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_WalkSlowActivate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_SwingActivate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CancelBy_InteractActivate);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Event_Activated);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Event_Commited);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Event_Failed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Event_Ended);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Event_Cancelled);

	// Collisions
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Collision);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Collision_None);

	// Set by caller
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Duration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Magnitude);

	// Cheats
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_Invulnerable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_NoTarget);
	
	// Movement states
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Jumping);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Crouching);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_WalkingSlow);

	// Capability states
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Stunned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Ragdoll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);

	// Damage states
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_IgnoreAnyDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_BlockAnyDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_EvadeAnyDamage);

	// Ability-driven states
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_FlashlightOn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Swinging);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Interacting);

	// Movement modes
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_None);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Walking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_NavWalking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Falling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Swimming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovementMode_Flying);

	// Cues
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Damage_Impact);
}
