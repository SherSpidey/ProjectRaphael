// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/ClimbAndVaultComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UClimbAndVaultComponent::UClimbAndVaultComponent():
Enabled(true),
MaxReachDistance(300),
RoomTolerance(1.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Add default query type for climb
	TraceTargets.Add(ObjectTypeQuery1);
	TraceTargets.Add(ObjectTypeQuery2);
	
	// Set default trace value
	LandTraceParam.TraceMinHeight = 50.f;
	LandTraceParam.TraceMaxHeight = 250.f;
	LandTraceParam.TraceForwardRadius = 30.f;
	LandTraceParam.TraceDownRadius = 30.f;

	AirTraceParam.TraceMinHeight = 125.f;
	AirTraceParam.TraceMaxHeight = 250.f;
	AirTraceParam.TraceForwardRadius = 30.f;
	AirTraceParam.TraceDownRadius = 30.f;
}


// Called when the game starts
void UClimbAndVaultComponent::BeginPlay()
{
	Super::BeginPlay();

	if(!Enabled)
	{
		return;
	}
	
	InitOwner();
}

void UClimbAndVaultComponent::InitOwner()
{
	// Init owner info
	Owner = Cast<ACharacter>(GetOwner());
	if(Owner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClimbAndVaultComponent has no valid owner!"))
		return ; 
	}
	if(Owner->GetCapsuleComponent() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClimbAndVaultComponent's owner has no valid capsule component!"))
		return ; 
	}
	OwnerHalfHeight = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	OwnerRadius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}


// Called every frame
void UClimbAndVaultComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UClimbAndVaultComponent::Climb()
{
	if(!Enabled)
	{
		return;
	}
	
	if(Owner == nullptr)
	{
		InitOwner();
	}

	bool bSuccess;
	if(Owner->GetMovementComponent()->IsFalling())
	{
		bSuccess = CheckForClimb(AirTraceParam, TargetTransform);
	}
	else
	{
		bSuccess = CheckForClimb(LandTraceParam, TargetTransform);
	}
	// Current can not climb
	if(!bSuccess)
	{
		return;
	}
	
	ActualStartTransformOffset.SetLocation(Owner->GetActorLocation() - TargetTransform.GetLocation());
}

bool UClimbAndVaultComponent::CheckForClimb_Implementation(FTraceParam& TraceParam, FTransform& FinishTransform)
{
	// Prepare the first trace to detect whether block by object
	const FVector OwnerForward = Owner->GetActorForwardVector();
	const FTransform ActualStart = Owner->GetTransform();

	// Set the trace capsule param
	const float TraceCapsuleHeight = (TraceParam.TraceMinHeight + TraceParam.TraceMaxHeight) / 2.f;
	const float TraceHalfHeight = (TraceParam.TraceMaxHeight - TraceParam.TraceMinHeight) / 2.f;
	
	// The mid point of max and min climb height
	const FVector TraceStartLocation = FVector(ActualStart.GetLocation().X, ActualStart.GetLocation().Y, ActualStart.GetLocation().Z - OwnerHalfHeight + 2.f) + FVector(0, 0, TraceCapsuleHeight);
	const FVector TraceEndLocation = OwnerForward * MaxReachDistance + TraceStartLocation;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Owner);
	FHitResult FrontTraceHit;
	bool bBlocked = UKismetSystemLibrary::CapsuleTraceSingleForObjects(this, TraceStartLocation, TraceEndLocation,
		TraceParam.TraceForwardRadius, TraceHalfHeight, TraceTargets, false, IgnoredActors, ClimbDebugType,
		FrontTraceHit, true);
	
	// Trace failed
	if(!bBlocked)
	{
		return false;
	}
	//UKismetSystemLibrary::DrawDebugSphere(this, FrontTraceHit.Location, 30, 12, FLinearColor::Black, 10, 1);
	
	// Second trace to test the climb point exists
	const FVector BlockLocation = FrontTraceHit.ImpactPoint;
	const FVector HitDirection = FrontTraceHit.ImpactNormal * -1.f;
	const FVector DownTraceEndLocation = FVector(BlockLocation.X, BlockLocation.Y, ActualStart.GetLocation().Z - OwnerHalfHeight + 2.f) + HitDirection * 10.f;
	const FVector DownTraceStartLocation = DownTraceEndLocation + FVector(0, 0,   TraceParam.TraceMaxHeight + TraceParam.TraceDownRadius);
	FHitResult DownTraceHit;
	bool bCanClimb = UKismetSystemLibrary::SphereTraceSingleForObjects(this, DownTraceStartLocation, DownTraceEndLocation,
		TraceParam.TraceDownRadius, TraceTargets, false, IgnoredActors, ClimbDebugType,
		DownTraceHit, true);
	
	// Second trace failed
	if(!bCanClimb)
	{
		return false;
	}
	
	const FVector DownHitLocation = DownTraceHit.ImpactPoint;
	//const FVector DownHitNorm = DownTraceHit.ImpactNormal;
	//UKismetSystemLibrary::DrawDebugLine(this, DownHitLocation, DownHitLocation + DownHitNorm * 50, FLinearColor::Green, 10, 1);
	
	const FVector CheckEndLocation = DownHitLocation + FVector(0, 0, OwnerRadius * RoomTolerance + 2.f);
	const FVector CheckStartLocation = CheckEndLocation + FVector(0, 0, 2 * (OwnerHalfHeight - OwnerRadius * RoomTolerance));
	
	bool bNoRoom = UKismetSystemLibrary::SphereTraceSingleForObjects(this, CheckStartLocation, CheckEndLocation,
		OwnerRadius * RoomTolerance, TraceTargets, false, IgnoredActors, ClimbDebugType,
		DownTraceHit, true, FLinearColor::Red, FLinearColor::Blue, 10);

	// Has no room to stand
	if(bNoRoom)
	{
		return false;
	}
	
	const FVector TargetLocation = DownHitLocation + FVector(0, 0, OwnerHalfHeight);
	const FRotator TargetRotation = UKismetMathLibrary::Conv_VectorToRotator(HitDirection);
	const float HeightOffset = TargetLocation.Z - ActualStart.GetLocation().Z;

	// Wrong situation for climb 
	if(HeightOffset < TraceParam.TraceMinHeight || HeightOffset > TraceParam.TraceMaxHeight)
	{
		return false;
	}
	
	FinishTransform.SetLocation(TargetLocation);
	FinishTransform.SetRotation(FQuat(TargetRotation));
	//UKismetSystemLibrary::DrawDebugSphere(this, FinishTransform.GetLocation(), 30, 12, FLinearColor::Black, 10, 1);
	
	return true;
}


