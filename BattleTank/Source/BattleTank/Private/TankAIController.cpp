// Fill out your copyright notice in the Description page of Project Settings.

#include "public/TankAIController.h"
#include "public/TankAimingComponent.h"
#include "Tank.h" // So we can implement OnDeath

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay() 
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		UE_LOG(LogTemp, Warning, TEXT("PossessedTank succesful"));

		// TODO Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!ensure(GetPawn())) { return; } // TODO remove if ok
	GetPawn()->DetachFromControllerPendingDestroy();

	UE_LOG(LogTemp, Warning, TEXT("Received!"))
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());


	// if aim or locked
	if (AimingComponent->GetFiringState() == EFiringState::Locked) 
	{
		AimingComponent->Fire(); // TODO limit firing rate
	}
	
}