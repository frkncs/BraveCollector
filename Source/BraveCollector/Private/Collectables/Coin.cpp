// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "BraveCollector/BraveCollectorCharacter.h"

void ACoin::Interact(ABraveCollectorCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	Super::Interact(InteractedCharacter);

	InteractedCharacter->OnSetCoinCount.Broadcast(InteractedCharacter->GetCoin() + IncreaseCoinAmount);
}
