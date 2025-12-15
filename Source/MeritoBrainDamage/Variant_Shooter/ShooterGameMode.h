// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

/**
 * Simple GameMode for a first person shooter game
 * Keeps track of team scores
 */
UCLASS(abstract)
class MERITOBRAINDAMAGE_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	/** Map of scores by team ID */
	TMap<uint8, int32> TeamScores;

protected:

	/** Gameplay initialization */
	virtual void BeginPlay() override;

public:

	/** Increases the score for the given team */
	void IncrementTeamScore(uint8 TeamByte);
};