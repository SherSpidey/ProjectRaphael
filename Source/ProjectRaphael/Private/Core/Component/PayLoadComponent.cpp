// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/PayLoadComponent.h"
#include "RParticle/RaphaelParticle.h"

// Sets default values for this component's properties
UPayLoadComponent::UPayLoadComponent():
ParticleNum(0),
MaxParticleNum(5),
PayLoadRadius(50.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPayLoadComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPayLoadComponent::DropParticle(int DropId)
{
	if(DropId >= ParticleNum)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invid Particle ID!"))
		return ;
	}
	// Get particle
	ARaphaelParticle* Particle = PayLoads[DropId].Particle;

	if(Particle)
	{
		Particle->DropItself();
	}
	
	// No matter valid or not, remove the id info
	PayLoads.RemoveAt(DropId);
	ParticleNum--;
}

void UPayLoadComponent::UpdateDistribution()
{
	if(ParticleNum == 0)
	{
		return ;
	}
	const FVector Right = GetRightVector();
	const FVector Up = GetUpVector();
	const FVector InitPosition = GetComponentLocation() + Right * PayLoadRadius;

	// The first particle position is always fixed
	PayLoads[0].Position = InitPosition;
	if(ParticleNum == 1)
	{
		return ;
	}
	// calculate gap
	const float Angular = PI / (ParticleNum - 1);
	for(int i = 1; i < ParticleNum; ++i)
	{
		const FVector ParticleDirection = Right * cos(Angular) + Up * sin(Angular);
		PayLoads[i].Position = ParticleDirection * PayLoadRadius;
	}
}


// Called every frame
void UPayLoadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	// ...
}

void UPayLoadComponent::LoadParticle(ARaphaelParticle* Particle)
{
	
}

