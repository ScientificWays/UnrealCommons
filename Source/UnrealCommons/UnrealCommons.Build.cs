// Scientific Ways

using UnrealBuildTool;

public class UnrealCommons : ModuleRules
{
	public UnrealCommons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
			
		});

		PublicDependencyModuleNames.AddRange(new string[] {

			"Core",
			"EngineSettings",

			"AIModule",
			"Navmesh",
			"NavigationSystem",

			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",

            "InputCore",
            "EnhancedInput",
            "UMG",
            "SlateCore",

            "Niagara",

            "Networking",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Engine",
			"CoreUObject",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
