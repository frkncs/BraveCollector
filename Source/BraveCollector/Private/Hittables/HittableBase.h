// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HittableBase.generated.h"

UCLASS()
class AHittableBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AHittableBase();

	UFUNCTION()
	virtual void OnHit(ABraveCollectorCharacter* InteractedCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	class USphereComponent* HitCollider;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* DestroyedParticle;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
};

