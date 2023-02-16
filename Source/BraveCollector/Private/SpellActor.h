// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellActor.generated.h"

UCLASS()
class ASpellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellActor();
	void SetFromFiredCharacter(class ABraveCollectorCharacter* NewFromFiredCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHitCollisionCollided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DestroyActor();

protected:
	UPROPERTY(EditAnywhere, Category="SpellActorProperties")
	class UNiagaraComponent* Particle;

	UPROPERTY(EditAnywhere, Category="SpellActorProperties")
	class USphereComponent* HitCollision;

	UPROPERTY(EditAnywhere, Category="SpellActorProperties")
	float MovementSpeed = 130;

	UPROPERTY(EditAnywhere, Category="SpellActorProperties")
	float LifeTimeInSeconds = 5;

private:
	ABraveCollectorCharacter* FromFiredCharacter;
	float LifeTimeTimer;
};
