// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "ShooterController.generated.h"


/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class AShooterCharacter;
struct FInputActionValue;

UCLASS()
class SIMPLESHOOTER_API AShooterController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	void StartSprint();
	void EndSprint();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Sprint;
	
	UPROPERTY(EditAnywhere, Category="Movement")
	FVector2D ControllerRotationSpeed = FVector2D(50.f, 50.f);

	AShooterCharacter* ControlledCharacter;
};
