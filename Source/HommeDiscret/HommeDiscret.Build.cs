// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HommeDiscret : ModuleRules
{
	public HommeDiscret(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"UMG"});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate",
			"SlateCore"});
	}
}
