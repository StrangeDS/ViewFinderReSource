// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ViewFinderReTarget : TargetRules
{
	public ViewFinderReTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("ViewFinderRe");
		ExtraModuleNames.AddRange(new string[] { "ViewFinderCore", "ViewFinderDemo" });
	}
}
