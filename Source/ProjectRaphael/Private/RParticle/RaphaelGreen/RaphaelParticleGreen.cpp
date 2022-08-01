// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelGreen/RaphaelParticleGreen.h"

#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/SplineComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARaphaelParticleGreen::ARaphaelParticleGreen():
CollapseRadius(300.f),
ThrowInitForce(1.f),
ThrowMaxForce(1.5f),
UpdateTrailStepTime(0.01f)
{
	TrailMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("TrailMovement"));
	TrailMovement->SetAutoActivate(false);

	TrailSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrailSpline"));
	TrailSpline->SetupAttachment(GetRootComponent());

	ParticleState = EGreenParticleState::EPS_Idle;
	ParticleType = EParticleType::EPT_Green;
	
}

void ARaphaelParticleGreen::SetThrowForce(float Force)
{
	ThrowForce = Force;
}

void ARaphaelParticleGreen::ParticleActive_Implementation()
{
	if(PlayerCharacter == nullptr)
	{
		GetPlayerCharacter();
	}
	if(PlayerCharacter)
	{
		if(ParticleState == EGreenParticleState::EPS_Idle)
		{
			BeginAiming();
		}
		else if(ParticleState == EGreenParticleState::EPS_Aiming)
		{
			bIsLoad = false;
			SetChosenReaction(false);
			OnParticlePendingActive();
			
			ThrowParticle();
		}
	}
}

void ARaphaelParticleGreen::Collapse_Implementation()
{
	ParticleState = EGreenParticleState::EPS_Collapse;
}

void ARaphaelParticleGreen::ThrowParticle_Implementation()
{
	ParticleState = EGreenParticleState::EPS_Moving;
}

void ARaphaelParticleGreen::BeginAiming_Implementation()
{
	ParticleState = EGreenParticleState::EPS_Aiming;
	
	// Start aiming
	GetWorldTimerManager().SetTimer(ShowTrailHandle, this, &ARaphaelParticleGreen::ShowTrail, UpdateTrailStepTime, true);
	
}
