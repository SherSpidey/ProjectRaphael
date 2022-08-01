// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RaphaelParticlePawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RParticle/RaphaelParticle.h"

// Sets default values
ARaphaelParticlePawn::ARaphaelParticlePawn():
bIsFalling(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPivot"));
	SetRootComponent(CameraRoot);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CameraRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	
}

// Called when the game starts or when spawned
void ARaphaelParticlePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARaphaelParticlePawn::UpdateFalling()
{
	const FVector Start { GetActorLocation() };
	const FVector End {Start + FVector::DownVector * 500.f};
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);
	const FVector ParticleScale = ControlledParticle->GetActorScale();
	if(HitResult.bBlockingHit && HitResult.Distance <= (ParticleScale.X * 1.1f))
	{
		bIsFalling = false;
	}
	else
	{
		bIsFalling = true;
	}
	
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
		UpdateFalling();
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

void ARaphaelParticlePawn::SetControlParticle_Implementation(ARaphaelParticle* Particle)
{
	ControlledParticle = Particle;
}

