// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/ClimbAndVaultComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UClimbAndVaultComponent::UClimbAndVaultComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClimbAndVaultComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UClimbAndVaultComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UClimbAndVaultComponent::CheckForClimb()
{
	ACharacter* Onwer = Cast<ACharacter>(GetOwner());
	float HalfHeight = Onwer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float Radius = Onwer->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

