// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AreadosTestTarget : TargetRules
{
	public AreadosTestTarget(TargetInfo Target) : base(Target)
	{
        //UE defaults

        Type = TargetType.Game;
		ExtraModuleNames.Add("AreadosTest");

        //Modules for HTTP | in replace of PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Http", "Json", "JsonUtilities" });
        /*ExtraModuleNames.AddRange(
            new string[]
            {
                "Http",
                "Json",
                "JsonUtilities"
            });
        */
        //ExtraModuleNames.Add("Json");
        //ExtraModuleNames.Add("JsonUtilities");

    }
}
