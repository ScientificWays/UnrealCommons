// Scientific Ways

#include "UnrealCommons.h"

#define LOCTEXT_NAMESPACE "FUnrealCommonsModule"

//----------------------------------------------------------------------//
// FGenericTeamId
//----------------------------------------------------------------------//
namespace
{
	ETeamAttitude::Type ScWTeamAttitudeSolver(FGenericTeamId InA, FGenericTeamId InB)
	{
		if (InA == FGenericTeamId::NoTeam || InB == FGenericTeamId::NoTeam)
		{
			return ETeamAttitude::Neutral;
		}
		else if (InA == InB)
		{
			return ETeamAttitude::Friendly;
		}
		else
		{
			return ETeamAttitude::Hostile;
		}
	}
}

void FUnrealCommonsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FGenericTeamId::SetAttitudeSolver(&ScWTeamAttitudeSolver);
}

void FUnrealCommonsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FGenericTeamId::ResetAttitudeSolver();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealCommonsModule, UnrealCommons)

DEFINE_LOG_CATEGORY(LogScWGameplay);
