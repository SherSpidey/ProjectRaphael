// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelSilver/RaphaelParticleSilver.h"

#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/RaphaelParticlePawn.h"
#include "Core/BasePlayerController.h"

ARaphaelParticleSilver::ARaphaelParticleSilver()
{
	
}

void ARaphaelParticleSilver::BackToPlayer()
{
	if(PlayerController)
	{
		if(PlayerCharacter)
		{
			OnParticleDeath.Broadcast();
			PlayerController->Possess(PlayerCharacter);
		}
	}
}

void ARaphaelParticleSilver::ParticleActive_Implementation()
{
	GetPlayerCharacter();
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
				}
			}
		}
	}
}
