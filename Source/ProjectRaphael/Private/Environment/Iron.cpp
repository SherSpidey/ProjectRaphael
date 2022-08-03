// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Iron.h"

// Sets default values
AIron::AIron()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IronMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagnetMesh"));
	SetRootComponent(IronMesh);
}

// Called when the game starts or when spawned
void AIron::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

