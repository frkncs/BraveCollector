// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"
#include "BraveCollector/BraveCollectorCharacter.h"

void AHealth::Interact(ABraveCollectorCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	Super::Interact(InteractedCharacter);

	int newHealth = InteractedCharacter->GetHealth() + giveHealth;
	newHealth = FMath::Min(newHealth, InteractedCharacter->GetMaxHealth());

	InteractedCharacter->PlayHealingAnim();

	InteractedCharacter->SetHealth(newHealth);
}