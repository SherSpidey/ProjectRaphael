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
	
}

// Called every frame
void ARaphaelParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARaphaelParticle::DropItself()
{
	
}

