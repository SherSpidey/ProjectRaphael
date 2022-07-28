// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/PayLoadComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "RParticle/RaphaelParticle.h"

// Sets default values for this component's properties
UPayLoadComponent::UPayLoadComponent():
CurrentIdx(-1),
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

void UPayLoadComponent::ParticleLoadFinish()
{
	const int ParticleNum = PayLoads.Num();
	if(ParticleNum == 1)
	{
		SetPickedParticle(0, false);
	}
	if(ParticleNum > MaxParticleNum)
	{
		// full then drop the first particle
		DropParticle(0);
		// The chosen particle has be removed
		if(CurrentIdx == 0)
		{
			// Reset
			SetPickedParticle(0, true);
		}
		else
		{
			PickPreviousParticle();
		}
	}
	ARaphaelParticle* Particle = PayLoads.Last().Particle;
	if(Particle)
	{
		// unbind
		Particle->OnTranslateFinish.RemoveAll(this);
	}
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
		Particle->SetChosenReaction(false);
		Particle->DropItself();
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

void UPayLoadComponent::SetPickedParticle(int Index, bool ReSet)
{
	// No particle loaded
	if(PayLoads.Num() == 0)
	{
		return ;
	}
	// Invalid Index
	if(Index < 0 || Index >= PayLoads.Num())
	{
		return ;
	}
	// Already set and don't need to reset
	if(!ReSet && Index == CurrentIdx)
	{
		return ;
	}
	// maybe need to adjust
	if(CurrentIdx >= 0 && CurrentIdx < PayLoads.Num())
	{
		PayLoads[CurrentIdx].Particle->SetChosenReaction(false);
	}
	CurrentIdx = Index;
	PayLoads[CurrentIdx].Particle->SetChosenReaction(true);
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
		
		// Bind finish delegate
		Particle->OnTranslateFinish.AddDynamic(this, &UPayLoadComponent::ParticleLoadFinish);

		// Lerp particle to position
		Particle->ApplyCurveToPosition();
	}
}

void UPayLoadComponent::PickNextParticle()
{
	if(CurrentIdx == PayLoads.Num() - 1)
	{
		SetPickedParticle(0, false);
	}
	else
	{
		SetPickedParticle(CurrentIdx + 1, false);
	}
}

void UPayLoadComponent::PickPreviousParticle()
{
	if(CurrentIdx == 0)
	{
		SetPickedParticle(PayLoads.Num() - 1, false);
	}
	else
	{
		SetPickedParticle(CurrentIdx - 1, false);
	}
}

