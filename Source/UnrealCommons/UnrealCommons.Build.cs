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
            "Networking",

            "Niagara",

            "InputCore",
            "EnhancedInput",
            "UMG",
            "SlateCore",

            "GameAnalytics",
        });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Engine",
			"CoreUObject",
		});
    }
}
