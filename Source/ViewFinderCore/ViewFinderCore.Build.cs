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
				"EnhancedInput",
				"PhysicsCore",
				"GeometryCore",
				"GeometryFramework",
				"DynamicMesh"
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"MeshConversionEngineTypes",
				"ModelingComponents"
			});

		PublicIncludePaths.AddRange(new string[] { "ViewFinderCore/Public" });

		PrivateIncludePaths.AddRange(new string[] { "ViewFinderCore/Private" });
		
		if (Target.bBuildEditor == true)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
	}
}
