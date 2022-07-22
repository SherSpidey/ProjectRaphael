// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelParticle.h"

// Sets default values
ARaphaelParticle::ARaphaelParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARaphaelParticle::BeginPlay()
{
	Super::BeginPlay();

	bIsLoad = false;
	
}

// Called every frame
void ARaphaelParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsLoad)
	{
		SetActorLocation(TargetPosition);
	}
}


void ARaphaelParticle::DropItself_Implementation()
{
	bIsLoad = false;
}

void ARaphaelParticle::ApplyCurveToPosition()
{
	
	SetActorLocation(TargetPosition);
	bIsLoad = true;
	OnInterpFinish.Broadcast(0);
}

