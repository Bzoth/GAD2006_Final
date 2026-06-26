// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAD2006_Final : ModuleRules
{
	public GAD2006_Final(ReadOnlyTargetRules Target) : base(Target)
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
			"GAD2006_Final",
			"GAD2006_Final/Variant_Platforming",
			"GAD2006_Final/Variant_Platforming/Animation",
			"GAD2006_Final/Variant_Combat",
			"GAD2006_Final/Variant_Combat/AI",
			"GAD2006_Final/Variant_Combat/Animation",
			"GAD2006_Final/Variant_Combat/Gameplay",
			"GAD2006_Final/Variant_Combat/Interfaces",
			"GAD2006_Final/Variant_Combat/UI",
			"GAD2006_Final/Variant_SideScrolling",
			"GAD2006_Final/Variant_SideScrolling/AI",
			"GAD2006_Final/Variant_SideScrolling/Gameplay",
			"GAD2006_Final/Variant_SideScrolling/Interfaces",
			"GAD2006_Final/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
