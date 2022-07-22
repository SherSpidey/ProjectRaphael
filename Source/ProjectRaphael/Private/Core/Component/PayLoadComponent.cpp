// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/PayLoadComponent.h"
#include "RParticle/RaphaelParticle.h"

// Sets default values for this component's properties
UPayLoadComponent::UPayLoadComponent():
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

	// WS
	ComponentLastPosition = GetComponentLocation();
	// ...
	
}

void UPayLoadComponent::DropParticle(int DropId)
{
	if(DropId >= PayLoads.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invid Particle ID!"))
		return ;
	}
	// Get particle
	ARaphaelParticle* Particle = PayLoads[DropId].Particle;

	if(Particle)
	{
		Particle->DropItself();
		Particle->OnInterpFinish.RemoveAll(this);
	}
	
	// No matter valid or not, remove the id info
	PayLoads.RemoveAt(DropId);
	UpdateDistribution();
}

void UPayLoadComponent::UpdateDistribution()
{
	if(PayLoads.Num() == 0)
	{
		return ;
	}
	const FVector Left = -GetRightVector();
	const FVector Up = GetUpVector();
	const FVector InitPosition = GetComponentLocation() + Left * PayLoadRadius;

	// The first particle position is always fixed
	PayLoads[0].Position = InitPosition;
	if(PayLoads.Num() == 1)
	{
		return ;
	}
	// calculate gap
	const float Angular = PI / (PayLoads.Num() - 1);
	for(int i = 1; i < PayLoads.Num(); ++i)
	{
		const FVector ParticleDirection = Left * cos(Angular) + Up * sin(Angular);
		PayLoads[i].Position = ParticleDirection * PayLoadRadius;
	}
}

void UPayLoadComponent::UpdateParticles()
{
	//Update component if self position
	const FVector DeltaPosition = GetComponentLocation() - ComponentLastPosition;
	ComponentLastPosition = GetComponentLocation();
	//Update particle
	for(FParticlePayloadInfo& ParticlePayloadInfo : PayLoads)
	{
		// Update in array
		ParticlePayloadInfo.Position += DeltaPosition;
		// Pass　to particles
		ARaphaelParticle* Particle = ParticlePayloadInfo.Particle;
		if(Particle)
		{
			Particle->UpdatePosition(ParticlePayloadInfo.Position);
		}
	}
}


// Called every frame
void UPayLoadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateParticles();
	// ...
}

void UPayLoadComponent::LoadParticle(ARaphaelParticle* Particle)
{
	if(Particle)
	{
		// Add to pay load
		FParticlePayloadInfo AddedParticleInfo;
		AddedParticleInfo.Particle = Particle;
		AddedParticleInfo.Position = FVector::ZeroVector;
		PayLoads.Add(AddedParticleInfo);

		// Update New Position
		UpdateDistribution();

		// Update to target position
		Particle->UpdatePosition(PayLoads.Last().Position);
		
		// Lerp particle to position
		Particle->ApplyCurveToPosition();

		if(PayLoads.Num() > MaxParticleNum)
		{
			// Bind delegate if full
			Particle->OnInterpFinish.AddDynamic(this, &UPayLoadComponent::UPayLoadComponent::DropParticle);
		}
	}
}

