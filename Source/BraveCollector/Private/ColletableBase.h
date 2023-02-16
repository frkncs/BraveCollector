// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColletableBase.generated.h"

UCLASS()
class AColletableBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColletableBase();
	virtual void Interact(ABraveCollectorCharacter* InteractedCharacter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void PlayParticle();

protected:
	UPROPERTY(EditAnywhere, Category="Mesh")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category="Mesh")
		class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, Category="Effect")
		class UNiagaraComponent* IdleParticle;

	UPROPERTY(EditAnywhere, Category="Effect")
		class UNiagaraSystem* DestroyParticle;
};
