// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Magnet.h"

#include "Components/SphereComponent.h"

// Sets default values
AMagnet::AMagnet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MagnetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagnetMesh"));
	SetRootComponent(MagnetMesh);

	AffectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AffectionSphere"));
	AffectionSphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AMagnet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagnet::SetMagnetSetting()
{
	switch(MagnetType)
	{
		case EMagnetType::EMT_Normal:
			//do nothing
			break;
		case EMagnetType::EMT_Fixed:
			// do nothing
			break;
		case EMagnetType::EMT_Platform:
			
			break;
		case EMagnetType::EMT_MAX: break;
		default: ;
	}
}

// Called every frame
void AMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

