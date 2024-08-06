// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	}
	
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (LineOfSightTo(PlayerPawn))
	{
		MoveToActor(PlayerPawn, AcceptanceRadius);
		SetFocus(PlayerPawn);
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		StopMovement();
	}
}