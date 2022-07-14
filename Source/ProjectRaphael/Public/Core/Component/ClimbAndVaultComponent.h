// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ClimbAndVaultComponent.generated.h"

USTRUCT(BlueprintType)
struct FClimbInfo
{
	GENERATED_USTRUCT_BODY()

	// Climb montage to play
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* ClimbMontage;

	// Climb curve to apply
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* ClimbCurve;

	// X, Y, Z move for anim root in climb montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector ClimbOffset;

	// Min climbable height
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinClimbHeight;

	// Max climbable height
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxClimbHeight;

	// The start time in climb curve when start with min climb height
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinClimbStartTime;

	// The start time in climb curve when start with max climb height
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxClimbStartTime;
	
};

USTRUCT(BlueprintType)
struct FTraceParam
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceMinHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceMaxHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceForwardRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceDownRadius;
	
};

UCLASS(ClassGroup=(Ability), Blueprintable, meta=(BlueprintSpawnableComponent) )
class PROJECTRAPHAEL_API UClimbAndVaultComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbAndVaultComponent();

protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* Owner;

	UPROPERTY(BlueprintReadOnly)
	float OwnerHalfHeight;

	UPROPERTY(BlueprintReadOnly)
	float OwnerRadius;
	
	UPROPERTY(BlueprintReadOnly)
	float ClimbHeight;

	// When the curve and climb start
	UPROPERTY(BlueprintReadOnly)
	float ClimbCurveStartPoint;

	// the curve played time
	UPROPERTY(BlueprintReadOnly)
	float ClimbCurvePlayBackTime;

	// the curve's Duration
	UPROPERTY(BlueprintReadOnly)
	float ClimbUpdateDuration;

	UPROPERTY(BlueprintReadWrite)
	bool bIsClimbing;

	UPROPERTY(BlueprintReadWrite)
	FTransform TargetTransform;

	UPROPERTY(BlueprintReadWrite)
	FVector ActualStartLocationOffset;

	UPROPERTY(BlueprintReadWrite)
	FRotator ActualStartRotation;

	UPROPERTY(BlueprintReadWrite)
	FVector AnimStartLocationOffset;

	UPROPERTY(BlueprintReadOnly)
	FClimbInfo CurrentClimbInfo;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle UpdateTimerHandle;

	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	bool Enabled;

	UPROPERTY(BlueprintReadOnly, Category="ClimbInfo")
	bool bSuccess;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	float MaxReachDistance;

	// This is a adjustment for room check !!!!!!!!!
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	float RoomTolerance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	float ClimbRate;

	// The curve play step time, useless when you override start function
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	float ClimbCurvePlayStepTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	FClimbInfo LowClimbInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	FClimbInfo HighClimbInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	FTraceParam LandTraceParam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClimbInfo")
	FTraceParam AirTraceParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ClimbInfo")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ClimbInfo")
	TEnumAsByte<EDrawDebugTrace::Type> ClimbDebugType;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitOwner();

	void InTimerCall();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Climb")
	bool CheckForClimb(FTraceParam& TraceParam, FTransform& FinishTransform);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Climb")
	FClimbInfo GetClimInfo();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Climb")
	void ClimbPreparation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Climb")
	void StartClimb();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Climb")
	void UpdateClimb(float TimePlayed, float BlendIn);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Climb();

	UFUNCTION(BlueprintCallable)
	void SetFunctionEnabled(bool Enable) { Enabled = Enable;}

	// Call when climb start
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Custom")
	void SetOwnerStatus();
};
