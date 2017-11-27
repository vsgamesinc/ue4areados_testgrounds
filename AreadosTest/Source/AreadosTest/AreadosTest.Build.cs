// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AreadosTest : ModuleRules
{
	public AreadosTest(ReadOnlyTargetRules Target) : base(Target)
	{
        //add some compiler modifers
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        //PCHUsage = PCHUsageMode.UseSharedPCHs;
        bEnforceIWYU = false;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "HeadMountedDisplay"
            });

        // Add Http Modules for AHttpLayer
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                    "Http",
                    "Json",
                    "JsonUtilities"
            });
    }
}
