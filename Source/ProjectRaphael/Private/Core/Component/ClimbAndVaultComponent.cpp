// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Component/ClimbAndVaultComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UClimbAndVaultComponent::UClimbAndVaultComponent():
bIsClimbing(false),
Enabled(true),
bSuccess(false),
MaxReachDistance(30.f),
RoomTolerance(1.f),
ClimbRate(1.f),
ClimbCurvePlayStepTime(0.0001f)
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

//Chose which climb action to use, can be override in BP
FClimbInfo UClimbAndVaultComponent::GetClimInfo_Implementation()
{
	// Climb info not set, error 
	if(LowClimbInfo.MaxClimbHeight == 0 || HighClimbInfo.MaxClimbHeight == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ClimbInfo has no valid data!"))
	}
	if(ClimbHeight >= HighClimbInfo.MinClimbHeight)
	{
		// Temp to check climbable
		bSuccess = HighClimbInfo.MaxClimbHeight > 0.f;
		return HighClimbInfo;
	}
	// Temp to check climbable
	bSuccess = LowClimbInfo.MaxClimbHeight > 0.f;
	return LowClimbInfo;
}

// Init the start climb info and param
void UClimbAndVaultComponent::ClimbPreparation_Implementation()
{
	
	CurrentClimbInfo = GetClimInfo();
	// Set actual location
	ActualStartLocationOffset = Owner->GetActorLocation() - TargetTransform.GetLocation();
	ActualStartRotation = Owner->GetActorRotation();

	// Set anim location offset
	const FVector ClimbDirection = TargetTransform.GetRotation().GetForwardVector();
	const FVector AnimLocation = TargetTransform.GetLocation() - FVector(ClimbDirection.X * CurrentClimbInfo.ClimbOffset.Y, ClimbDirection.Y * CurrentClimbInfo.ClimbOffset.Y, CurrentClimbInfo.ClimbOffset.Z);
	AnimStartLocationOffset = AnimLocation - TargetTransform.GetLocation();

	ClimbCurveStartPoint = UKismetMathLibrary::MapRangeClamped(ClimbHeight, CurrentClimbInfo.MinClimbHeight, CurrentClimbInfo.MaxClimbHeight, CurrentClimbInfo.MinClimbStartTime, CurrentClimbInfo.MaxClimbStartTime);

	float CurveStartTime;
	float CurveEndTime;
	CurrentClimbInfo.ClimbCurve->GetTimeRange(CurveStartTime, CurveEndTime);
	ClimbUpdateDuration = CurveEndTime - ClimbCurveStartPoint;
}

// Update the owner's transform along with the climb animation
void UClimbAndVaultComponent::UpdateClimb_Implementation(float TimePlayed, float BlendIn)
{
	// Climb failed~
	if(!bSuccess)
	{
		return ;
	}
	
	// PositionAlpha:CurveValue.X, X/Y-Alpha:CurveValue.Y, Z-Alpha:CurveValue.Z
	const FVector CurveValue = CurrentClimbInfo.ClimbCurve->GetVectorValue(TimePlayed + ClimbCurveStartPoint);

	// Lerp XY location
	const FVector XYLocation = FVector(AnimStartLocationOffset.X, AnimStartLocationOffset.Y, ActualStartLocationOffset.Z);
	const FVector LerpXYLocation = UKismetMathLibrary::VLerp(ActualStartLocationOffset, XYLocation, CurveValue.Y);

	// Lerp Z location
	const float LerpZLocation = UKismetMathLibrary::Lerp(ActualStartLocationOffset.Z, AnimStartLocationOffset.Z, CurveValue.Z);

	// Make the lerp offset
	const FVector LerpLocationOffset =FVector(LerpXYLocation.X, LerpXYLocation.Y, LerpZLocation);

	// Lerp the wanted target location
	const FVector LerpTargetLocation = UKismetMathLibrary::VLerp(TargetTransform.GetLocation() +  LerpLocationOffset, TargetTransform.GetLocation(), CurveValue.X);

	// Lerp the final update owner location
	const FVector UpdateOwnerLocation = UKismetMathLibrary::VLerp(ActualStartLocationOffset + TargetTransform.GetLocation(), LerpTargetLocation, BlendIn);

	// Lerp the final update owner rotation
	const FRotator UpdateOwnerRotation = UKismetMathLibrary::RLerp(ActualStartRotation, FRotator(TargetTransform.GetRotation()), BlendIn, true);

	// Set the final transform to owner
	Owner->SetActorTransform(FTransform(UpdateOwnerRotation, UpdateOwnerLocation, FVector::OneVector));
}

// Best to overrider in BP to use timeline
void UClimbAndVaultComponent::StartClimb_Implementation()
{
	const UWorld* World = GetWorld();
	if(World)
	{
		// Update climb transform
		bIsClimbing = true;
		World->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UClimbAndVaultComponent::InTimerCall, ClimbCurvePlayStepTime, true);
		ClimbCurvePlayBackTime = 0.f;

		// Play anim montage
		Owner->GetMesh()->GetAnimInstance()->Montage_Play(CurrentClimbInfo.ClimbMontage, ClimbRate, EMontagePlayReturnType::Duration, ClimbCurveStartPoint, true);
	}
}

// Call by timer
void UClimbAndVaultComponent::InTimerCall()
{
	ClimbCurvePlayBackTime += ClimbCurvePlayStepTime;
	if(ClimbCurvePlayBackTime >= ClimbUpdateDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
		bIsClimbing = false;
	}
	const float BlendIn = UKismetMathLibrary::MapRangeClamped(ClimbCurvePlayBackTime, 0.f, 0.2f, 0, 1);
	UpdateClimb(ClimbCurvePlayBackTime, BlendIn);
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
	// Already in climbing
	if(bIsClimbing)
	{
		return ;
	}
	
	//Check for climb
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
	// Set Climb Start
	ClimbPreparation();

	// Set Timer to start update
	StartClimb();

	// Set Owner Status
	SetOwnerStatus();
}

// Override in BP to set owner status
void UClimbAndVaultComponent::SetOwnerStatus_Implementation()
{
	//Change OwnerStatus here
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
	const FVector HitDirection = FVector(FrontTraceHit.ImpactNormal.X, FrontTraceHit.ImpactNormal.Y, 0) * -1.f;
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
	// UKismetSystemLibrary::DrawDebugLine(this, DownHitLocation, DownHitLocation + HitDirection * 50, FLinearColor::Green, 10, 1);
	// UKismetSystemLibrary::DrawDebugSphere(this, DownHitLocation, 5, 12, FLinearColor::Black, 10, 1);
	
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
	ClimbHeight = HeightOffset;
	FinishTransform.SetLocation(TargetLocation);
	FinishTransform.SetRotation(FQuat(TargetRotation));
	FinishTransform.SetScale3D(FVector::OneVector);
	
	//UKismetSystemLibrary::DrawDebugSphere(this, FinishTransform.GetLocation(), 30, 12, FLinearColor::Black, 10, 1);
	
	return true;
}


