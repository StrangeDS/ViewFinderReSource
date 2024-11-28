// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ViewFinderCore : ModuleRules
{
	public ViewFinderCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"GeometryCore",
				"GeometryScriptingCore",
				"UMG"
			});

		PublicIncludePaths.AddRange(new string[] { "ViewFinderCore/Public" });

		PrivateIncludePaths.AddRange(new string[] { "ViewFinderCore/Private" });
	}
}
