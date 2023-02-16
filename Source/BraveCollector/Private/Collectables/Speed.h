// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColletableBase.h"
#include "Speed.generated.h"

/**
 * 
 */
UCLASS()
class ASpeed : public AColletableBase
{
	GENERATED_BODY()

public:
	virtual void Interact(ABraveCollectorCharacter* InteractedCharacter) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Collectable Properties")
		float newMaxSpeed = 700.f;
	UPROPERTY(EditAnywhere, Category = "Collectable Properties")
		float duration = 2.f;
};
