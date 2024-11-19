// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ViewFinderReEditorTarget : TargetRules
{
	public ViewFinderReEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("ViewFinderRe");
		ExtraModuleNames.AddRange(new string[] { "ViewFinderCore", "ViewFinderDemo" });
	}
}
