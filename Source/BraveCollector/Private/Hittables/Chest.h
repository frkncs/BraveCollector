// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HittableBase.h"
#include "Chest.generated.h"

/**
 * 
 */
UCLASS()
class AChest : public AHittableBase
{
	GENERATED_BODY()

public:
	virtual void OnHit(ABraveCollectorCharacter* InteractedCharacter) override;

protected:
	UPROPERTY(EditAnywhere, Category="Hittable Properties")
	int IncreaseCoinAmount = 15;
};
