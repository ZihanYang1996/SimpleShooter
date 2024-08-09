// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root; // or SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Fire()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fire!"));
	if (!bIsFiring)
	{
		bIsFiring = true;
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AGun::SpawnBullet, FireRate, true, 0.f);
	}
}

void AGun::StopFire()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Stop Fire!"));
	if (bIsFiring)
	{
		bIsFiring = false;
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
	}
}

void AGun::SpawnBullet()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spawn Bullet!"));
	// Spawn muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	// Line trace
	FVector Location;
	FRotator Rotation;
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (AController* Controller = OwnerCharacter->GetController())
		{
			Controller->GetPlayerViewPoint(Location, Rotation);
		}
	}

	FVector End = Location + Rotation.Vector() * 10000.f;
	FHitResult HitResult;

	// Ignore owner
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End,
	                                                   ECollisionChannel::ECC_GameTraceChannel1, Params);

	// Draw debug sphere at hit location
	// if (bIsHit)
	// {
	// 	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false, 2.f);
	// }

	// Spawn impact effect
	if (bIsHit)
	{
		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAttached(ImpactEffect, HitResult.GetComponent(), HitResult.BoneName,
		                                       HitResult.ImpactPoint, ShotDirection.Rotation(),
		                                       EAttachLocation::KeepWorldPosition);
		// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint, ShotDirection.Rotation());

		// Apply damage
		// UDamageType::StaticClass() or nullptr
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, UDamageType::StaticClass());
		// GetOwner()->GetInstigatorController() or Controller from above
		if (AActor* HitActor = HitResult.GetActor())
		{
			HitActor->TakeDamage(Damage, DamageEvent, GetOwner()->GetInstigatorController(), this);
		}
	}
}
