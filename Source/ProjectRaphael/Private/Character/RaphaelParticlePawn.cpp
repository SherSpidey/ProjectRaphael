// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RaphaelParticlePawn.h"

#include "RParticle/RaphaelParticle.h"

// Sets default values
ARaphaelParticlePawn::ARaphaelParticlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARaphaelParticlePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARaphaelParticlePawn::ReleaseControl_Implementation()
{
	ControlledParticle = nullptr;
	Destroy();
}

// Finish it via BP
void ARaphaelParticlePawn::FunctionActivate_Implementation()
{
}

void ARaphaelParticlePawn::FunctionDeactivate_Implementation()
{
	
}

// Called every frame
void ARaphaelParticlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ControlledParticle)
	{
		SetActorLocation(ControlledParticle->GetActorLocation());
	}

}

// Called to bind functionality to input
void ARaphaelParticlePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &ARaphaelParticlePawn::ReleaseControl);

	PlayerInputComponent->BindAction("ItemHold", IE_Pressed, this, &ARaphaelParticlePawn::FunctionActivate);
	PlayerInputComponent->BindAction("ItemRelease", IE_Released, this, &ARaphaelParticlePawn::FunctionDeactivate);

}

