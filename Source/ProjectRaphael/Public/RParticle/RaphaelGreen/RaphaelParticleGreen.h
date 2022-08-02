// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RParticle/RaphaelParticle.h"
#include "RaphaelParticleGreen.generated.h"

UENUM(BlueprintType)
enum class EGreenParticleState: uint8
{
	EPS_Idle UMETA(DisplayName = "Idle"),
	EPS_Aiming UMETA(DisplayName = "Aiming"),
	EPS_Moving UMETA(DisplayName = "Moving"),
	EPS_Collapse UMETA(DisplayName = "Collapse"),
	
	EPS_MAX UMETA(DisplayName = "Default")
};

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticleGreen : public ARaphaelParticle
{
	GENERATED_BODY()

	ARaphaelParticleGreen();
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	float CollapseRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	float ThrowInitForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	float ThrowMaxForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	float UpdateTrailStepTime;

protected:
	UPROPERTY(BlueprintReadWrite, Category=Function)
	EGreenParticleState ParticleState;

	UPROPERTY(BlueprintReadWrite, Category=Setting)
	float ThrowForce;

	UPROPERTY(BlueprintReadWrite, Category=Setting)
	FTimerHandle ShowTrailHandle;
	
	UPROPERTY(BlueprintReadWrite, Category=Setting)
	TArray<class USplineMeshComponent*> SplineMeshes;

	UPROPERTY(BlueprintReadWrite, Category=Setting)
	FVector InitVelocity;

	UPROPERTY(BlueprintReadWrite, Category=Setting)
	TArray<AActor*> CapturedActors;
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* TrailMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class USplineComponent* TrailSpline;

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Function)
	void ShowTrail();

	UFUNCTION(BlueprintCallable, Category=Function)
	void SetThrowForce(float Force);

	UFUNCTION(BlueprintCallable, Category=Function)
	void ClearLastTrail();

	UFUNCTION(BlueprintCallable, Category=Function)
	void SetTrailPath(TArray<FVector> Positions);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Function)
	USplineMeshComponent* CreateSplineMesh();

	UFUNCTION(BlueprintCallable, Category=Function)
	void CalculateInitVelocity();

	UFUNCTION(BlueprintCallable, Category=Function)
	void UnstableCollapse(const FHitResult& ImpactResult);

	UFUNCTION(BlueprintCallable, Category=Function)
	bool CaptureActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void DestroyCapturedActors();

public:
	
	virtual void ParticleActive_Implementation() override;

	virtual void ParticleSetFunctionEnable_Implementation(bool Enabled) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void BeginAiming();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void ThrowParticle();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void Collapse();
};
