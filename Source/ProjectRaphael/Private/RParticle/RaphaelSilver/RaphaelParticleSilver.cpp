// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelSilver/RaphaelParticleSilver.h"

#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/RaphaelParticlePawn.h"
#include "Components/SphereComponent.h"
#include "Core/BasePlayerController.h"

ARaphaelParticleSilver::ARaphaelParticleSilver():
JumpInitPower(2000.f),
JumpMaxPower(4000.f),
MovingForce(500.f)
{
	JumpPower = JumpInitPower;
	ParticleType = EParticleType::EPT_Silver;
}

void ARaphaelParticleSilver::BackToPlayer_Implementation()
{
	if(PlayerController)
	{
		if(PlayerCharacter)
		{
			OnParticleDeath.Broadcast();
			PlayerController->Possess(PlayerCharacter);
			
			PlayerController->SetHUDVisibility(true);

			// back to normal
			ParticleMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
			ParticleMesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
			ActionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

// Using timeline, implement in BP
void ARaphaelParticleSilver::JumpHold_Implementation()
{
	
}

void ARaphaelParticleSilver::ParticleActive_Implementation()
{
	if(PlayerCharacter == nullptr)
	{
		GetPlayerCharacter();
	}
	if(PlayerCharacter)
	{
		AController* BaseController = PlayerCharacter->GetController();
		if(BaseController)
		{
			PlayerController = Cast<ABasePlayerController>(BaseController);
			if(PlayerController)
			{
				bIsLoad = false;
				SetChosenReaction(false);
				ParticleMesh->SetSimulatePhysics(true);
				const FVector Force = PlayerCharacter->GetFollowCamera()->GetForwardVector() * 100000.f;
				OnParticleActive.Broadcast();
				ParticleMesh->AddForce(Force, "", true);
				if(ControllerPawn)
				{
					ControllerPawn->SetControlParticle(this);
					PlayerController->Possess(ControllerPawn);
					PlayerController->SetHUDVisibility(false);
				}
			}
		}
	}
}

void ARaphaelParticleSilver::Jump_Implementation(FVector Direction)
{
	ParticleMesh->AddForce(Direction * JumpPower, "", true);
}

void ARaphaelParticleSilver::Rolling_Implementation(FVector Direction, float Scale)
{
	ParticleMesh->AddForce(Direction * Scale * MovingForce, "", true);
}
