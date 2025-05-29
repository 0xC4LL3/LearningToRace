// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LearningToRace : ModuleRules
{
	public LearningToRace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Landscape", "InputCore", "EnhancedInput", "ChaosVehicles", "PhysicsCore", "LearningAgents", "LearningTraining" });
	}
}
