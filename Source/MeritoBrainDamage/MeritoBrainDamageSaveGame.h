#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MeritoBrainDamageSaveGame.generated.h"

/**
 * Container for all saved data (Settings, Progress, etc.)
 */
UCLASS()
class MERITOBRAINDAMAGE_API UMeritoBrainDamageSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UMeritoBrainDamageSaveGame();

    // The variable we want to remember
    UPROPERTY(VisibleAnywhere, Category = "Audio")
    float MasterVolume;
};