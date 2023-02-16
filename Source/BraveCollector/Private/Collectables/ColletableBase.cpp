// Fill out your copyright notice in the Description page of Project Settings.


#include "ColletableBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"

// Sets default values
AColletableBase::AColletableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetRelativeScale3D(FVector(2.f));

	IdleParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("IdleParticle"));
	IdleParticle->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AColletableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AColletableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColletableBase::Interact(ABraveCollectorCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	PlayParticle();
	Destroy();
}

void AColletableBase::PlayParticle()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestroyParticle, GetActorLocation());
}

