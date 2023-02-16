// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColletableBase.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class ACoin : public AColletableBase
{
	GENERATED_BODY()

public:
	virtual void Interact(ABraveCollectorCharacter* InteractedCharacter) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="Collectable Properties")
	int IncreaseCoinAmount = 1;
};
