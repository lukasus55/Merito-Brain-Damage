// Fill out your copyright notice in the Description page of Project Settings.


#include "MeritoBrainDamageVideoSettings.h"
#include "Kismet/KismetSystemLibrary.h" // For console commands if needed

UGameUserSettings* UMeritoBrainDamageVideoSettings::GetSettings() const
{
    return GEngine ? GEngine->GetGameUserSettings() : nullptr;
}

void UMeritoBrainDamageVideoSettings::SetWindowMode(int32 Index)
{
    if (UGameUserSettings* Settings = GetSettings())
    {
        switch (Index)
        {
        case 0: Settings->SetFullscreenMode(EWindowMode::Fullscreen); break;
        case 1: Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen); break; // Borderless
        case 2: Settings->SetFullscreenMode(EWindowMode::Windowed); break;
        }
    }
}

void UMeritoBrainDamageVideoSettings::SetVSync(bool bEnabled)
{
    if (UGameUserSettings* Settings = GetSettings())
    {
        Settings->SetVSyncEnabled(bEnabled);
    }
}

void UMeritoBrainDamageVideoSettings::ApplyVideoSettings()
{
    if (UGameUserSettings* Settings = GetSettings())
    {
        // This applies resolution, fullscreen mode, vsync, etc.
        // AND saves to GameUserSettings.ini automatically.
        Settings->ApplySettings(false);
    }
}

int32 UMeritoBrainDamageVideoSettings::GetCurrentWindowModeIndex() const
{
    if (UGameUserSettings* Settings = GetSettings())
    {
        EWindowMode::Type Mode = Settings->GetFullscreenMode();
        switch (Mode)
        {
        case EWindowMode::Fullscreen: return 0;
        case EWindowMode::WindowedFullscreen: return 1;
        case EWindowMode::Windowed: return 2;
        default: return 2;
        }
    }
    return 2; // Default to windowed if fails
}

bool UMeritoBrainDamageVideoSettings::GetCurrentVSyncState() const
{
    if (UGameUserSettings* Settings = GetSettings())
    {
        return Settings->IsVSyncEnabled();
    }
    return false;
}

void UMeritoBrainDamageVideoSettings::SetMaxFrameRate(int32 NewLimit)
{
    if (UGameUserSettings* Settings = GetSettings())
    {
        Settings->SetFrameRateLimit(NewLimit);
    }
}

int32 UMeritoBrainDamageVideoSettings::GetCurrentFrameRateLimit() const
{
    if (UGameUserSettings* Settings = GetSettings())
    {
        return Settings->GetFrameRateLimit();
    }
    return 0; // 0 = Unlimited
}