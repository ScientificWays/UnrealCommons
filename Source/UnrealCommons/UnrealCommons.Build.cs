// Scientific Ways

using UnrealBuildTool;

public class UnrealCommons : ModuleRules
{
	public UnrealCommons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//PublicIncludePaths.AddRange(new string[]
		//{
		//    "UnrealCommons",
		//});

		PublicDependencyModuleNames.AddRange(new string[] {

			"Core",
            "CoreUObject",
            "EngineSettings",

			"InputCore",
			"EnhancedInput",
			"UMG",

			"AIModule",
			"Navmesh",
			"NavigationSystem",

			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",

			"Networking",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Engine",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
