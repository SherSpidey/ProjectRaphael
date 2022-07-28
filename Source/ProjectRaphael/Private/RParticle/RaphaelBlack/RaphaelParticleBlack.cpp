// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelBlack/RaphaelParticleBlack.h"

#include "Character/BaseCharacter.h"
#include "Core/Component/PayLoadComponent.h"

// Get the hit item
void ARaphaelParticleBlack::GetTargetActor()
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

void ARaphaelParticleBlack::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                                AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                const FHitResult& SweepHitResult)
{
	Super::OnSphereBeginOverlap_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                           SweepHitResult);
}

void ARaphaelParticleBlack::ParticleActive_Implementation()
{
	//Super::ParticleActive_Implementation();
	bIsLoad = false;
	SetChosenReaction(false);
	GetTargetActor();
}
