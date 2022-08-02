// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelBR/RaphaelParticleBR.h"

ARaphaelParticleBR::ARaphaelParticleBR()
{
	MagnetType = EBRParticleType::EPT_South;
	ParticleType = EParticleType::EPT_BR;
	ParticleState = EBRParticleState::EPS_Idle;
	
	ParticleMesh->SetVisibility(false);
	
}

void ARaphaelParticleBR::BeginPlay()
{
	
	
	Super::BeginPlay();
}
