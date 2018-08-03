// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class PointCloudSelectingEditorTarget : TargetRules
{
	public PointCloudSelectingEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "PointCloudSelecting" } );
	}
}
