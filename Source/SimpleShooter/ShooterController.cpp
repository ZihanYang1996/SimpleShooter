// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterCharacter.h"

void AShooterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Get the pawn that the controller is controlling


	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this,
		                                   &AShooterController::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this,
		                                   &AShooterController::Look);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this,
		                                   &AShooterController::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this,
		                                   &AShooterController::StopJumping);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AShooterController::StartSprint);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AShooterController::EndSprint);
	}
}

void AShooterController::BeginPlay()
{
	Super::BeginPlay();
	ControlledCharacter = Cast<AShooterCharacter>(GetCharacter());
	if (!ControlledCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ControlledCharacter is not valid"));
	}
}

void AShooterController::Move(const FInputActionValue& Value)
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Move(Value);
	}
}

void AShooterController::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	AddYawInput(LookInput.X * ControllerRotationSpeed.X * GetWorld()->DeltaTimeSeconds);
	AddPitchInput(LookInput.Y * ControllerRotationSpeed.Y * GetWorld()->DeltaTimeSeconds);
}

void AShooterController::Jump()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Jump"));
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AShooterController::StopJumping()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopJumping"));
	if (ControlledCharacter)
	{
		ControlledCharacter->StopJumping();
	}
}

void AShooterController::StartSprint()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StartSprint();
	}
}

void AShooterController::EndSprint()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->EndSprint();
	}
}