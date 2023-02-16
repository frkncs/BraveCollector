// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BraveCollectorCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetCoinCountSignature, int, NewCoinCount);

UCLASS(config=Game)
class ABraveCollectorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABraveCollectorCharacter();
	void SetSpeed(float newSpeed, float duration);

	FSetCoinCountSignature OnSetCoinCount;

	// GETTERS
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE int GetHealth() { return Health; }
	FORCEINLINE int GetMaxHealth() { return maxHealth; }
	FORCEINLINE int GetCoin() { return Coin; }

	//SETTERS
	UFUNCTION()
		void SetHealth(int NewHealth);
	UFUNCTION()
		void SetCoinCount(int NewCoinCount);

	float PlayAttackAnim();
	void PlayThrowSpellAnim();
	void PlayHealingAnim();
	void PlayHitAnim();

	UFUNCTION(BlueprintImplementableEvent)
		void ShowWinUI();
	UFUNCTION(BlueprintImplementableEvent)
		void ShowLoseUI();
	

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// To add mapping context
	virtual void BeginPlay();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();

	UFUNCTION()
	void CapsuleCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
			
	UFUNCTION(BlueprintCallable, Category="Interact")
	void OnThrowSpellNotify();

	UFUNCTION(BlueprintCallable, Category="Interact")
	void OnAttackNotifyCalled();

	UFUNCTION(BlueprintCallable, Category="Interact")
	void OnAttackNotifyEndedCalled();

	UFUNCTION(BlueprintCallable, Category="Interact")
	void OnHittedHittableCalled(class AHittableBase* hittedHittable);

private:
	void SetSpeedEnd();
	void SetIsAttackFalse();

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ThrowSpellAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* HealingAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* HitAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* ThrowSpellAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* AttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* ThrowSpellParticleStartPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ASpellActor> SpellBP;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Actor)
		int maxHealth = 100;

private:

	class UAnimInstance* AnimInstance;

	class ABraveCollectorGameMode* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Actor, meta = (AllowPrivateAccess = "true"))
	int Coin;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Actor, meta = (AllowPrivateAccess = "true"))
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Actor, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	FTimerHandle th_SpeedTimer;
	FTimerHandle th_SetIsAttackFalse;

	float _defaultMaxWalkSpeed;
};

