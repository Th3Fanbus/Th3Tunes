/* SPDX-License-Identifier: MPL-2.0 */

#include "Th3Tunes.h"
#include "Wwise/WwiseSoundBankManagerImpl.h"
#include "Wwise/WwiseSoundBankFileState.h"
#include "Patching/NativeHookManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogTh3TunesCpp, Log, All);

/*
 * In order for this to compile, a header edit is necessary.
 * 
 *   File:    Wwise/WwiseSoundBankManagerImpl.h
 *   Class:   FWwiseSoundBankManagerImpl
 *   Friend:  FTh3WwisePatcher
 */
class FTh3WwisePatcher
{
public:
	static void CreateOp(
		TCallScope<FWwiseFileStateSharedPtr(__cdecl*)(FWwiseSoundBankManagerImpl*, const FWwiseSoundBankCookedData&, const FString&)>& scope,
		FWwiseSoundBankManagerImpl* self,
		const FWwiseSoundBankCookedData& InSoundBankCookedData,
		const FString& InRootPath
	)
	{
		/* Mods MUST name their sound banks `Mod_MODREFERENCE.bnk` for them to be remapped properly */
		FString BankName = InSoundBankCookedData.SoundBankPathName.ToString();
		if (BankName.RemoveFromStart(TEXT("Mod_"))) {
			if (BankName.RemoveFromEnd(TEXT(".bnk"))) {
				const FString& ModReference = BankName;
				const FString NewRoot = FString::Printf(TEXT("../../../FactoryGame/Mods/%s/Content/WwiseAudio"), *ModReference);
				UE_LOG(LogTh3TunesCpp, Warning, TEXT("Remapping mod %s bank path from %s to %s"), *ModReference, *InRootPath, *NewRoot);
				UE_LOG(LogTh3TunesCpp, Warning, TEXT("    For sound bank: %s"), *InSoundBankCookedData.GetDebugString());
				scope.Override(scope(self, InSoundBankCookedData, NewRoot));
			}
		}
	}

	static void InstallHook()
	{
		SUBSCRIBE_METHOD_VIRTUAL(FWwiseSoundBankManagerImpl::CreateOp, new FWwiseSoundBankManagerImpl(), &FTh3WwisePatcher::CreateOp);
	}
};

void FTh3TunesModule::StartupModule()
{
	if (!WITH_EDITOR) {
		FTh3WwisePatcher::InstallHook();
	}
}

void FTh3TunesModule::ShutdownModule()
{
}
	
IMPLEMENT_MODULE(FTh3TunesModule, Th3Tunes)