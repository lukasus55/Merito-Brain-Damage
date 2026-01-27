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
        true             // Apply to existing sounds immediately
    );

    UGameplayStatics::PushSoundMixModifier(GetWorld(), GlobalSoundMix);
}