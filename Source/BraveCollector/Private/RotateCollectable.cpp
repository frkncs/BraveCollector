// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateCollectable.h"

// Sets default values for this component's properties
URotateCollectable::URotateCollectable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotateCollectable::BeginPlay()
{
	Super::BeginPlay();
	
	ParentActor = GetOwner();
}


// Called every frame
void URotateCollectable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator currentRotation = ParentActor->GetActorRotation();
	currentRotation.Yaw += rotateSpeed * DeltaTime;

	ParentActor->SetActorRotation(currentRotation);
}

