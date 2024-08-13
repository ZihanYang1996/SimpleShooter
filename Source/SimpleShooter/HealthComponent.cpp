// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DecreaseHealth(float Amount)
{
	Health -= Amount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
}

bool UHealthComponent::IsDead() const
{
	return Health <= 0.f;
}

float UHealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}