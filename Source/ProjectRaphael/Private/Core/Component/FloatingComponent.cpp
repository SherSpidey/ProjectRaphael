// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/FloatingComponent.h"

// Sets default values for this component's properties
UFloatingComponent::UFloatingComponent():
FloatingHeight(20.f)
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

	// ...
	
}

void UFloatingComponent::InitOwner()
{
	OwnerActor = GetOwner();
}

void UFloatingComponent::ApplyForce_Implementation()
{
	
}


// Called every frame
void UFloatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ApplyForce();
	// ...
}

