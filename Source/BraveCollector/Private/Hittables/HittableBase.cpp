// Fill out your copyright notice in the Description page of Project Settings.


#include "HittableBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AHittableBase::AHittableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	HitCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHittableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHittableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHittableBase::OnHit(ABraveCollectorCharacter* InteractedCharacter)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestroyedParticle, GetActorLocation());

	Destroy();
}

