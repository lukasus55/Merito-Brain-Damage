// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MeritoBrainDamage : ModuleRules
{
	public MeritoBrainDamage(ReadOnlyTargetRules Target) : base(Target)
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
			"MeritoBrainDamage",
			"MeritoBrainDamage/Variant_Horror",
			"MeritoBrainDamage/Variant_Horror/UI",
			"MeritoBrainDamage/Variant_Shooter",
			"MeritoBrainDamage/Variant_Shooter/AI",
			"MeritoBrainDamage/Variant_Shooter/UI",
			"MeritoBrainDamage/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
