// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ViewFinderDemo : ModuleRules
{
	public ViewFinderDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG"
			});

		PublicIncludePaths.AddRange(new string[] { "ViewFinderDemo/Public" });

		PrivateIncludePaths.AddRange(new string[] { "ViewFinderDemo/Private" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ViewFinderCore" });
	}
}
