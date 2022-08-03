// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelBR/RaphaelParticleBR.h"
#include "NiagaraComponent.h"
#include "Character/BaseCharacter.h"
#include "Environment/Magnet.h"

ARaphaelParticleBR::ARaphaelParticleBR()
{
	ControlParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ControlParticle"));
	ControlParticle->SetupAttachment(GetRootComponent());
	ControlParticle->SetAutoActivate(false);
	
	MagnetType = EMagnetPoleType::EMT_South;
	ParticleType = EParticleType::EPT_BR;
	ParticleState = EBRParticleState::EPS_Idle;
	
}

void ARaphaelParticleBR::BeginPlay()
{
	
	Super::BeginPlay();
}

void ARaphaelParticleBR::SetParticleMagnet()
{
	// Set material
	switch (MagnetType)
	{
		case EMagnetPoleType::EMT_North:
			ParticleMesh->SetMaterial(0, NorthMaterial);
			break;
		case EMagnetPoleType::EMT_South:
			ParticleMesh->SetMaterial(0, SouthMaterial);
			break;
		case EMagnetPoleType::EMT_Iron: break;
		case EMagnetPoleType::EMT_Nonmagnetic: break;
		case EMagnetPoleType::EMT_MAX: break;
		default: ;
	}
}

void ARaphaelParticleBR::CheckMagnet_Implementation()
{
	if(PlayerCharacter != nullptr)
	{
		AActor* TraceActor = PlayerCharacter->TraceForObjectOnce();
		if(TraceActor)
		{
			const AMagnet* Magnet = Cast<AMagnet>(TraceActor);
			if(Magnet)
			{
				ControlledActor = TraceActor;
			}
			else
			{
				ControlledActor = nullptr;
			}
		}
	}
}

void ARaphaelParticleBR::ControlMagnet_Implementation()
{
	if(ControlledActor)
	{
		const IInteractInterface* MagnetInterface = Cast<IInteractInterface>(ControlledActor);
		if(MagnetInterface)
		{
			MagnetInterface->Execute_SetMagnetPoleType(ControlledActor, MagnetType);
		}
	}
}

void ARaphaelParticleBR::DestroyEquippedParticle_Implementation()
{
	if(PlayerCharacter)
	{
		const IInteractInterface* MagnetInterface = Cast<IInteractInterface>(PlayerCharacter);
		if(MagnetInterface)
		{
			MagnetInterface->Execute_SetMagnetPoleType(PlayerCharacter, EMagnetPoleType::EMT_Nonmagnetic);
		}
	}
	OnParticlePendingKill();
	// Finish effect in BP!!!
}

void ARaphaelParticleBR::SetControlParticleMaterial_Implementation()
{
	if(MagnetType == EMagnetPoleType::EMT_North)
	{
		ControlParticle->SetVariableMaterial(FName("PoleMaterial"), NorthMaterial);
	}
	else if(MagnetType == EMagnetPoleType::EMT_South)
	{
		ControlParticle->SetVariableMaterial(FName("PoleMaterial"), SouthMaterial);
	}
}

void ARaphaelParticleBR::ParticleActive_Implementation()
{
	if(PlayerCharacter == nullptr)
	{
		GetPlayerCharacter();
	}
	if(PlayerCharacter)
	{
		if(ParticleState == EBRParticleState::EPS_Idle)
		{
			ParticleState = EBRParticleState::EPS_Function;
		}
		else if(ParticleState == EBRParticleState::EPS_Function)
		{
			CheckMagnet();
			if(ControlledActor)
			{
				bIsLoad = false;
				SetChosenReaction(false);
				OnParticlePendingActive();

				ParticleState = EBRParticleState::EPS_Control;
				ParticleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				StartLocation = GetActorLocation();
				BeginControl();
			}
		}
		else if(ParticleState == EBRParticleState::EPS_Equipped)
		{
			DestroyEquippedParticle();
		}
	}
}

void ARaphaelParticleBR::ParticleSetFunctionEnable_Implementation(bool Enabled)
{
	if(!Enabled)
	{
		return;
	}
	// Change magnet pole
	if(ParticleState == EBRParticleState::EPS_Function || ParticleState == EBRParticleState::EPS_Equipped)
	{
		if(MagnetType == EMagnetPoleType::EMT_South)
		{
			MagnetType = EMagnetPoleType::EMT_North;
		}
		else if (MagnetType == EMagnetPoleType::EMT_North)
		{
			MagnetType = EMagnetPoleType::EMT_South;
		}
		SetParticleMagnet();

		// Send change to player
		if(ParticleState == EBRParticleState::EPS_Equipped)
		{
			if(PlayerCharacter)
			{
				const IInteractInterface* MagnetInterface = Cast<IInteractInterface>(PlayerCharacter);
				if(MagnetInterface)
				{
					MagnetInterface->Execute_SetMagnetPoleType(PlayerCharacter, MagnetType);
					SetControlParticleMaterial();
				}
			}
		}
	}
}

void ARaphaelParticleBR::SpecialFunction_Implementation()
{
	if(ParticleState == EBRParticleState::EPS_Function)
	{
		bIsLoad = false;
		SetChosenReaction(false);
		OnParticlePendingActive();
		
		// Finish Effect In BP!!
		ParticleState = EBRParticleState::EPS_Equipped;
		StartLocation = GetActorLocation();
		
		if(PlayerCharacter)
		{
			const IInteractInterface* MagnetInterface = Cast<IInteractInterface>(PlayerCharacter);
			if(MagnetInterface)
			{
				MagnetInterface->Execute_SetMagnetPoleType(PlayerCharacter, MagnetType);
			}
		}
	}
}
