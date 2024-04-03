/* SPDX-License-Identifier: MPL-2.0 */

using UnrealBuildTool;
using System.IO;
using System;

public class Th3Tunes : ModuleRules
{
	public Th3Tunes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine",
            "DeveloperSettings", "PhysicsCore", "InputCore",
			"AssetRegistry", "RenderCore",
            "SlateCore", "Slate", "UMG",
            "DummyHeaders", "FactoryGame", "SML",
            "WwiseFileHandler", "AkAudio",
        });
    }
}
