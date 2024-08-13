// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UHealthComponent;
class AGun;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	void StartSprint();

	void EndSprint();

	void PullTrigger();

	void ReleaseTrigger();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

private:
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float SprintSpeed = 350.f;

	UPROPERTY(EditAnywhere, Category="Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category="Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Gun")
	TSubclassOf<AGun> GunClass;

	AGun* Gun;

	UHealthComponent* HealthComponent;
	

	UCharacterMovementComponent* CharacterMovement;
	
};
