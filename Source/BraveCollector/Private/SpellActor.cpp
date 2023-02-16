// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellActor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Hittables/HittableBase.h"
#include "ColletableBase.h"
#include "Collectables/Coin.h"
#include "BraveCollector/BraveCollectorCharacter.h"

// Sets default values
ASpellActor::ASpellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComponent"));
	Particle->SetupAttachment(GetRootComponent());

	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(GetRootComponent());
}

void ASpellActor::BeginPlay()
{
	Super::BeginPlay();

	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpellActor::OnHitCollisionCollided);
	
}

void ASpellActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTimeTimer += DeltaTime;

	if (LifeTimeTimer >= LifeTimeInSeconds)
	{
		DestroyActor();
		return;
	}

	FVector newLoc = GetActorLocation();
	newLoc += GetActorForwardVector() * (MovementSpeed * DeltaTime);

	SetActorLocation(newLoc);

}

void ASpellActor::SetFromFiredCharacter(ABraveCollectorCharacter* NewFromFiredCharacter)
{
	FromFiredCharacter = NewFromFiredCharacter;
}

void ASpellActor::OnHitCollisionCollided(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !FromFiredCharacter)
	{
		return;
	}

	AHittableBase* collidedHittable = Cast<AHittableBase>(OtherActor);
	ACoin* collidedCoin = Cast<ACoin>(OtherActor);

	if (!Cast<ABraveCollectorCharacter>(OtherActor) && collidedCoin)
	{
		DestroyActor();
	}

	if (!collidedHittable)
	{
		if (collidedCoin)
		{
			collidedCoin->Interact(FromFiredCharacter);
			return;
		}

		return;
	}

	collidedHittable->OnHit(FromFiredCharacter);

	DestroyActor();
}

void ASpellActor::DestroyActor()
{
	Destroy();
}

