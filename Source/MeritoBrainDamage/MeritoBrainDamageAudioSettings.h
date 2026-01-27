// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "MeritoBrainDamageSaveGame.h"
#include "MeritoBrainDamageAudioSettings.generated.h"

/**
 *
 */
UCLASS()
class MERITOBRAINDAMAGE_API UMeritoBrainDamageAudioSettings : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SetVolume(USoundClass* TargetClass, float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    void SaveAudioSettings(float CurrentVolume);

    UFUNCTION(BlueprintCallable, Category = "Audio Settings")
    float LoadAudioSettings();

    UFUNCTION(BlueprintCallable, Category = "Audio Global", meta = (WorldContext = "WorldContextObject"))
    static void InitializeAudioGlobal(const UObject* WorldContextObject, USoundMix* Mix, USoundClass* Class);

protected:
    // Helper to get the settings pointer easily
    UGameUserSettings* GetSettings() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings")
    USoundMix* GlobalSoundMix;
};
