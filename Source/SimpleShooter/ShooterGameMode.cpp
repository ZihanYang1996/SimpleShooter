// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupHUD();
}