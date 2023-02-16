// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage.h"
#include "BraveCollector/BraveCollectorCharacter.h"

void ADamage::Interact(ABraveCollectorCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	Super::Interact(InteractedCharacter);

	int newHealthValue = InteractedCharacter->GetHealth() - damageValue;
	newHealthValue = FMath::Max(0, newHealthValue);

	InteractedCharacter->PlayHitAnim();
	InteractedCharacter->SetHealth(newHealthValue);
}