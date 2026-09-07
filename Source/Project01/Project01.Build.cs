// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project01 : ModuleRules
{
	public Project01(ReadOnlyTargetRules Target) : base(Target)
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

        PublicIncludePaths.Add(ModuleDirectory);
    }
}
