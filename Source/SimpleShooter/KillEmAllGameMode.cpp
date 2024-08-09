// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

#include "EngineUtils.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
		DisplayLoseScreen();
	}
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		if (Controller->IsPlayerController())
		{
			// If the controller is a player controller, then bIsWinner should be the same as bIsPlayerWinner
			Controller->GameHasEnded(Controller->GetPawn(), bIsPlayerWinner);
		}
		else
		{
			// If the controller is not a player controller, then bIsWinner should be the opposite of bIsPlayerWinner
			Controller->GameHasEnded(Controller->GetPawn(), !bIsPlayerWinner);
		}
	}
}