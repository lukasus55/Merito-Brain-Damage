// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "MeritoBrainDamageVideoSettings.generated.h"

/**
 * 
 */
UCLASS()
class MERITOBRAINDAMAGE_API UMeritoBrainDamageVideoSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // --- SETTERS (Called by UI when you change a value) ---

    UFUNCTION(BlueprintCallable, Category = "Video Settings")
    void SetWindowMode(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Video Settings")
    void SetVSync(bool bEnabled);

    // --- APPLIER (Called by Save Button) ---

    UFUNCTION(BlueprintCallable, Category = "Video Settings")
    void ApplyVideoSettings();

    // --- GETTERS (Called on Construct to set UI state) ---

    UFUNCTION(BlueprintPure, Category = "Video Settings")
    int32 GetCurrentWindowModeIndex() const;

    UFUNCTION(BlueprintPure, Category = "Video Settings")
    bool GetCurrentVSyncState() const;

protected:
    // Helper to get the settings pointer easily
    UGameUserSettings* GetSettings() const;

};
