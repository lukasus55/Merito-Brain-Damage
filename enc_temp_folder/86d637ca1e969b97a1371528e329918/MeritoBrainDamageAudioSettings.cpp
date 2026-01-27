// Fill out your copyright notice in the Description page of Project Settings.


#include "MeritoBrainDamageAudioSettings.h"
#include "Kismet/KismetSystemLibrary.h" // For console commands if needed
#include "Kismet/GameplayStatics.h"

UGameUserSettings* UMeritoBrainDamageAudioSettings::GetSettings() const
{
    return GEngine ? GEngine->GetGameUserSettings() : nullptr;
}

void UMeritoBrainDamageAudioSettings::SetVolume(USoundClass* TargetClass, float Volume)
{
    if (!GlobalSoundMix || !TargetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("MeritoAudio: Missing SoundMix or TargetClass!"));
        return;
    }


    UGameplayStatics::SetSoundMixClassOverride(
        GetWorld(),
        GlobalSoundMix,  // The mix modifier
        TargetClass,     // The specific group (e.g., Master)
        Volume,          // New volume (0.0 - 1.0)
        1.0f,            // Pitch (1.0 = normal)
        0.0f,            // Fade in time (0.0 = instant)
        true             // Apply to children (not sure bout this one tbh)
    );

    UGameplayStatics::PushSoundMixModifier(GetWorld(), GlobalSoundMix);
}

void UMeritoBrainDamageAudioSettings::SaveAudioSettings(float CurrentVolume)
{
    // Create an instance of SaveGame object
    if (UMeritoBrainDamageSaveGame* SaveInst = Cast<UMeritoBrainDamageSaveGame>(UGameplayStatics::CreateSaveGameObject(UMeritoBrainDamageSaveGame::StaticClass())))
    {
        SaveInst->MasterVolume = CurrentVolume;

        UGameplayStatics::SaveGameToSlot(SaveInst, TEXT("Settings"), 0);

        UE_LOG(LogTemp, Log, TEXT("Audio Saved: %f"), CurrentVolume);
    }
}

float UMeritoBrainDamageAudioSettings::LoadAudioSettings()
{
    if (UGameplayStatics::DoesSaveGameExist(TEXT("Settings"), 0))
    {
        USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("Settings"), 0);

        if (UMeritoBrainDamageSaveGame* SaveInst = Cast<UMeritoBrainDamageSaveGame>(LoadedGame))
        {
            return SaveInst->MasterVolume;
        }
    }

    // Default to 0.6 if no save file exists
    return 0.6f;
}