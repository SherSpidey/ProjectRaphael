// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelBlack/RaphaelParticleBlack.h"
#include "NiagaraComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/Component/PayLoadComponent.h"

ARaphaelParticleBlack::ARaphaelParticleBlack():
bIsFunctional(false),
bControlled(false)
{
	ControlParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ControlParticle"));
	ControlParticle->SetupAttachment(GetRootComponent());
	ControlParticle->SetAutoActivate(false);

	ParticleType = EParticleType::EPT_White;
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
		AActor* PlayerActor = PayLoadComponent->GetOwner();
		if(PlayerActor != nullptr)
		{
			ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(PlayerActor);
			if(PlayerCharacter != nullptr)
			{
				TargetActor = PlayerCharacter->TraceForObjectOnce();
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
	ControlParticle->SetActive(true, true);

	ControlParticle->SetNiagaraVariableVec3(FString("InitVelocity"), InterpSpeed);
}

void ARaphaelParticleBlack::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                                AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                const FHitResult& SweepHitResult)
{
	if(TargetActor != nullptr)
	{
		if(OtherActor == TargetActor)
		{
			bControlled = true;
			ControlParticle->SetNiagaraVariableBool(FString("Instance"), true);
			GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString("Here"));
		}
	}
	Super::OnSphereBeginOverlap_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                           SweepHitResult);
}

// Finished in BP
void ARaphaelParticleBlack::ParticleActive_Implementation()
{
	//Super::ParticleActive_Implementation();
	bIsLoad = false;
	SetChosenReaction(false);
}

// Finished in BP
void ARaphaelParticleBlack::ParticleSetFunctionEnable_Implementation(bool Enabled)
{
	
}
