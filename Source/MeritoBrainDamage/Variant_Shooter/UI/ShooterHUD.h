// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterHUD.generated.h"

/**
 * Main HUD for the game.
 * Handles Ammo, Health, and other player feedback.
 */
UCLASS(abstract)
class MERITOBRAINDAMAGE_API UShooterHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Updates the ammo count on the HUD */
	UFUNCTION(BlueprintImplementableEvent, Category = "Shooter|HUD")
	void BP_UpdateBulletCounter(int32 MagazineSize, int32 BulletCount);

	/** Updates the health/damage effects */
	UFUNCTION(BlueprintImplementableEvent, Category = "Shooter|HUD")
	void BP_Damaged(float LifePercent);
};