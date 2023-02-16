// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BraveCollectorGameMode.generated.h"

UCLASS(minimalapi)
class ABraveCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABraveCollectorGameMode();

public:
	UPROPERTY(EditAnywhere, Category="Game Mode Settings")
	uint16 MaxScore = 50;
};
