// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DugeonEscape : ModuleRules
{
	public DugeonEscape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"DugeonEscape",
			"DugeonEscape/Variant_Horror",
			"DugeonEscape/Variant_Horror/UI",
			"DugeonEscape/Variant_Shooter",
			"DugeonEscape/Variant_Shooter/AI",
			"DugeonEscape/Variant_Shooter/UI",
			"DugeonEscape/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
