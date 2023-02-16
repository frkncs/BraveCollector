// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"
#include "BraveCollector/BraveCollectorCharacter.h"

void AChest::OnHit(ABraveCollectorCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	Super::OnHit(InteractedCharacter);

	InteractedCharacter->OnSetCoinCount.Broadcast(InteractedCharacter->GetCoin() + IncreaseCoinAmount);
}