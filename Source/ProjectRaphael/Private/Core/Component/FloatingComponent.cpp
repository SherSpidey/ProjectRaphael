// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/FloatingComponent.h"

// Sets default values for this component's properties
UFloatingComponent::UFloatingComponent():
bSpawned(false),
bActivated(true),
FloatingHeight(30.f),
ForceScale(5.f),
LoseControlRadius(20.f),
SpawnCount(1),
SpawnCoolDownTime(2.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
	
}


// Called when the game starts
void UFloatingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitOwner();
	// ...
	
}

void UFloatingComponent::InitOwner()
{
	OwnerActor = GetOwner();
}

void UFloatingComponent::ResetSpawn()
{
	bSpawned = false;
	const UWorld* World = GetWorld();
	if(World)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void UFloatingComponent::ApplyForce_Implementation()
{
	if(FloatingActor == nullptr)
	{
		return ;
	}
	const FVector TargetLocation = FloatingActor->GetActorLocation();
	FVector DeltaPosition = TargetLocation - LandLocation;
	const float DeltaXY = FVector2D(DeltaPosition.X, DeltaPosition.Y).Size();
	if(DeltaXY > LoseControlRadius)
	{
		FloatingActor = nullptr;
		const UWorld* World = GetWorld();
		if(World)
		{
			World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UFloatingComponent::ResetSpawn, SpawnCoolDownTime, false);
		}
		return ;
	}
	if(DeltaPosition.Z <= 0)
	{
		DeltaPosition.Z = 0;
	}
	if(DeltaPosition.Z >= FloatingHeight)
	{
		DeltaPosition.Z = FloatingHeight;
	}
	// Get Target Mesh
	UActorComponent* ActorComponent = FloatingActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
	if(ActorComponent)
	{
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
		if(MeshComponent)
		{
			const FVector Force = FVector::UpVector * ForceScale * FMath::Pow((FloatingHeight - DeltaPosition.Z)/ FloatingHeight, 3) * 1000;
			if(MeshComponent->IsSimulatingPhysics())
			{
				MeshComponent->AddForce(Force, "", true);
			}
		}
	}
}


void UFloatingComponent::SetLandPosition_Implementation(FVector Location)
{
	LandLocation = Location;
}

void UFloatingComponent::SetActiveFloatingSpawn(bool Enable)
{
	bActivated = Enable;
}

// Called every frame
void UFloatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bActivated)
	{
		SpawnSetParticle();
		ApplyForce();
	}else if(FloatingActor)
	{
		FloatingActor = nullptr;
		const UWorld* World = GetWorld();
		if(World)
		{
			World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UFloatingComponent::ResetSpawn, SpawnCoolDownTime, false);
		}
	}
	// ...
}

void UFloatingComponent::SpawnSetParticle_Implementation()
{
	if(!bSpawned && SpawnCount > 0)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			FloatingActor = World->SpawnActor<AActor>(SpawnParticle, LandLocation + FVector(0, 0, FloatingHeight), FRotator(0, 0, 0));
			if(FloatingActor)
			{
				bSpawned = true;
				SpawnCount --;
			}
		}
	}
}

