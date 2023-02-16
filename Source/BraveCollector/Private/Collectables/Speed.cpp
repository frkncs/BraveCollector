// Fill out your copyright notice in the Description page of Project Settings.


#include "Speed.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BraveCollector/BraveCollectorCharacter.h"

void ASpeed::Interact(ABraveCollectorCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	Super::Interact(InteractedCharacter);

	InteractedCharacter->SetSpeed(newMaxSpeed, duration);
}