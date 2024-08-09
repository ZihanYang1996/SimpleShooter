// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void SetupHUD();

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void DisplayLoseScreen();

	UFUNCTION(BlueprintImplementableEvent, Category="HUD")
	void DisplayWinScreen();	

	virtual void PawnKilled(APawn* PawnKilled);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="My Widgets")
	UUserWidget* CurrentWidgetOnViewport;
};
