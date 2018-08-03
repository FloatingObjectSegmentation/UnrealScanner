// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class PointCloudSelectingTarget : TargetRules
{
	public PointCloudSelectingTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] {
            "PointCloudSelecting"
        } );
	}
}
