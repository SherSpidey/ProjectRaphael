// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/PayLoadComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RParticle/RaphaelParticle.h"

// Sets default values for this component's properties
UPayLoadComponent::UPayLoadComponent():
MaxParticleNum(5),
PayLoadRadius(50.f),
MaxRotationUpdateLimit(2.f)
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

	OriginRadius = PayLoadRadius;
	LastPayloadRadius = PayLoadRadius;
	
	// WS
	ComponentLastPosition = GetComponentLocation();
	ComponentLastRotation = GetComponentRotation();
	
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
		Particle->OnTranslateFinish.RemoveAll(this);
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
	const FVector Left = -1 * GetRightVector();
	const FVector Up = GetUpVector();
	const FVector Origin = GetComponentLocation();
	const FVector InitPosition = Origin + Left * PayLoadRadius;

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
		const FVector ParticleDirection = Left * cos(Angular * i) + Up * sin(Angular * i);
		PayLoads[i].Position = Origin + ParticleDirection * PayLoadRadius;
	}
}

void UPayLoadComponent::UpdateParticles()
{
	bAdjusted = false;
	if(PayLoads.Num() == 0)
	{
		// Still need to update location
		ComponentLastPosition = GetComponentLocation();
		return ;
	}
	//Update component position change
	const FVector DeltaPosition = GetComponentLocation() - ComponentLastPosition;
	ComponentLastPosition = GetComponentLocation();

	// Check Rotation change
	const FRotator DeltaRotation = GetComponentRotation() - ComponentLastRotation;
	
	// exceed the limit to update
	if(abs(DeltaRotation.Yaw) > MaxRotationUpdateLimit)
	{
		UpdateDistribution();
		bAdjusted = true;
		ComponentLastRotation = GetComponentRotation();
	}

	// Check radius change
	if(LastPayloadRadius != PayLoadRadius)
	{
		if(!bAdjusted)
		{
			UpdateDistribution();
		}
		LastPayloadRadius = PayLoadRadius;
	}
	
	//Update particle
	for(FParticlePayloadInfo& ParticlePayloadInfo : PayLoads)
	{
		// Update in array
		ParticlePayloadInfo.Position += DeltaPosition;
		// Pass　to particles
		ARaphaelParticle* Particle = ParticlePayloadInfo.Particle;
		if(Particle)
		{
			Particle->UpdatePosition(ParticlePayloadInfo.Position, ComponentLastPosition);
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
		Particle->UpdatePosition(PayLoads.Last().Position, ComponentLastPosition);

		if(PayLoads.Num() > MaxParticleNum)
		{
			// Bind delegate if full
			Particle->OnTranslateFinish.AddDynamic(this, &UPayLoadComponent::DropParticle);
		}

		// Lerp particle to position
		Particle->ApplyCurveToPosition();
		
	}
}

