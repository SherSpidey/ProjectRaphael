// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelBlack/RaphaelParticleBlack.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/Component/PayLoadComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

ARaphaelParticleBlack::ARaphaelParticleBlack():
ControlDistance(600.f),
ControlPressDistance(180.f),
PressForceScale(1.f)
{
	ControlParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ControlParticle"));
	ControlParticle->SetupAttachment(GetRootComponent());
	ControlParticle->SetAutoActivate(false);

	ControlHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("ControlHandle"));

	ParticleType = EParticleType::EPT_Black;
	ParticleState = EBlackParticleState::EPS_Idle;

	CurrentDistance = ControlDistance;
}

void ARaphaelParticleBlack::CalculateSpeed(float InterpTime)
{
	StartPosition = GetActorLocation();
	if(TargetActor)
	{
		InterpSpeed = (TargetActor->GetActorLocation() - StartPosition) / InterpTime;
	}
}

// Get the hit item
void ARaphaelParticleBlack::GetTargetActor()
{
	if(PayLoadComponent)
	{
		GetPlayerCharacter();
		if(PlayerCharacter != nullptr)
		{
			AActor* TraceActor = PlayerCharacter->TraceForObjectOnce();
			if(TraceActor && TraceActor->ActorHasTag(FName("Controllable")))
			{
				TargetActor = TraceActor;
			}
		}
	}
}

void ARaphaelParticleBlack::Transformer()
{

	// Setting for mesh
	ParticleMesh->SetVisibility(false);
	ParticleMesh->SetSimulatePhysics(false);
	ParticleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActionArea->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	ControlParticleInit();
}

void ARaphaelParticleBlack::ControlParticleInit_Implementation()
{
	ControlParticle->SetNiagaraVariableVec3(FString("InitVelocity"), InterpSpeed);

	ControlParticle->SetActive(true, true);
}

void ARaphaelParticleBlack::ControlTarget()
{
	if(PlayerCharacter != nullptr)
	{
		const UCameraComponent* Camera = PlayerCharacter->GetFollowCamera();
		if(Camera != nullptr)
		{
			const FVector ControlLocation = Camera->GetForwardVector() * CurrentDistance + Camera->GetComponentLocation();
			ControlHandle->SetTargetLocation(ControlLocation);
		}
	}
}

// maybe add something later
void ARaphaelParticleBlack::OnControlParticleDeathFinish()
{
	// Destroy the whole actor
	ParticleDeath();
}

void ARaphaelParticleBlack::ApplyPressForce()
{
	FVector Direction;
	if(PlayerCharacter != nullptr)
	{
		const UCameraComponent* Camera = PlayerCharacter->GetFollowCamera();
		if(Camera != nullptr)
		{
			Direction = Camera->GetForwardVector();
		}
	}
	else
	{
		Direction = GetActorForwardVector();
	}
	const FVector Force = (ControlDistance - CurrentDistance) * PressForceScale * Direction * 1000.f;

	if(TargetActor)
	{
		USceneComponent* Root = TargetActor->GetRootComponent();
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Root);
		if(Mesh)
		{
			Mesh->AddForce(Force, "", true);
		}
	}
}

// Finished in BP
void ARaphaelParticleBlack::ParticleActive_Implementation()
{
	if(PlayerCharacter == nullptr)
	{
		GetPlayerCharacter();
	}
	if(PlayerCharacter)
	{
		bIsLoad = false;
		SetChosenReaction(false);
	}
}

// Finished in BP
void ARaphaelParticleBlack::ParticleSetFunctionEnable_Implementation(bool Enabled)
{
	
}
