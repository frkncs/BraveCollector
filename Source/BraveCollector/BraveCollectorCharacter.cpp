// Copyright Epic Games, Inc. All Rights Reserved.

#include "BraveCollectorCharacter.h"
#include "BraveCollectorGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Private/InteractInterface.h"
#include "Animation/AnimMontage.h"
#include "Private/Hittables/HittableBase.h"
#include "Private/SpellActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// ABraveCollectorCharacter

ABraveCollectorCharacter::ABraveCollectorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(GetRootComponent());

	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetRootComponent());

	ThrowSpellParticleStartPos = CreateDefaultSubobject<USceneComponent>(TEXT("ThrowSpellParticleStartPos"));
	ThrowSpellParticleStartPos->SetupAttachment(GetRootComponent());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ABraveCollectorCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GameMode = Cast<ABraveCollectorGameMode>(UGameplayStatics::GetGameMode(this));

	OnSetCoinCount.AddDynamic(this, &ABraveCollectorCharacter::SetCoinCount);

	Health = maxHealth;
	_defaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &ABraveCollectorCharacter::CapsuleCollisionBeginOverlap);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AnimInstance = GetMesh()->GetAnimInstance();
		
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABraveCollectorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABraveCollectorCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABraveCollectorCharacter::Look);

		//Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ABraveCollectorCharacter::Attack);
		EnhancedInputComponent->BindAction(ThrowSpellAction, ETriggerEvent::Started, this, &ABraveCollectorCharacter::PlayThrowSpellAnim);
	}

}

void ABraveCollectorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABraveCollectorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABraveCollectorCharacter::SetCoinCount(int NewCoinCount)
{
	Coin = NewCoinCount;

	if (Coin >= GameMode->MaxScore)
	{
		UGameplayStatics::OpenLevel(this, "Map01");
	}
}

float ABraveCollectorCharacter::PlayAttackAnim()
{
	return AnimInstance->Montage_Play(AttackAnim, 1, EMontagePlayReturnType::Duration);
}

void ABraveCollectorCharacter::PlayThrowSpellAnim()
{
	if (bIsAttacking)
	{
		return;
	}

	bIsAttacking = true;

	float duration = AnimInstance->Montage_Play(ThrowSpellAnim, 1, EMontagePlayReturnType::Duration);

	GetWorldTimerManager().SetTimer(th_SetIsAttackFalse, this, &ABraveCollectorCharacter::SetIsAttackFalse, duration);
}

void ABraveCollectorCharacter::PlayHealingAnim()
{
	AnimInstance->Montage_Play(HealingAnim);
}

void ABraveCollectorCharacter::PlayHitAnim()
{
	AnimInstance->Montage_Play(HitAnim);
}

void ABraveCollectorCharacter::Attack()
{
	if (bIsAttacking)
	{
		return;
	}

	bIsAttacking = true;
	float duration = PlayAttackAnim();

	GetWorldTimerManager().SetTimer(th_SetIsAttackFalse, this, &ABraveCollectorCharacter::SetIsAttackFalse, duration);
}

void ABraveCollectorCharacter::CapsuleCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		IInteractInterface* CollectableActor = Cast<IInteractInterface>(OtherActor);

		if (CollectableActor)
		{
			CollectableActor->Interact(this);
		}
	}
}

void ABraveCollectorCharacter::SetSpeed(float newSpeed, float duration)
{
	GetCharacterMovement()->MaxWalkSpeed = newSpeed;

	GetWorldTimerManager().SetTimer(th_SpeedTimer, this, &ABraveCollectorCharacter::SetSpeedEnd, duration);	
}

void ABraveCollectorCharacter::SetSpeedEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = _defaultMaxWalkSpeed;
	GetWorldTimerManager().ClearTimer(th_SpeedTimer);
}

void ABraveCollectorCharacter::SetIsAttackFalse()
{
	bIsAttacking = false;

	GetWorldTimerManager().ClearTimer(th_SetIsAttackFalse);

	OnAttackNotifyEndedCalled();
}

void ABraveCollectorCharacter::OnThrowSpellNotify()
{
	FVector spawnPos = ThrowSpellParticleStartPos->GetComponentTransform().GetLocation();
	FRotator spawnRot = UKismetMathLibrary::FindLookAtRotation(spawnPos, spawnPos + GetActorForwardVector() * 30);

	ASpellActor* spawnedActor = GetWorld()->SpawnActor<ASpellActor>(SpellBP, spawnPos, spawnRot);
	spawnedActor->SetFromFiredCharacter(this);
}

void ABraveCollectorCharacter::OnAttackNotifyCalled()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABraveCollectorCharacter::OnAttackNotifyEndedCalled()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABraveCollectorCharacter::OnHittedHittableCalled(AHittableBase* hittedHittable)
{
	hittedHittable->OnHit(this);
}