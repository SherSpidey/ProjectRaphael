// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelGreen/RaphaelParticleGreen.h"

#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARaphaelParticleGreen::ARaphaelParticleGreen():
CollapseRadius(300.f),
ThrowInitForce(1.f),
ThrowMaxForce(1.5f),
UpdateTrailStepTime(0.01f)
{
	TrailMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("TrailMovement"));
	TrailMovement->SetAutoActivate(false);

	TrailSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrailSpline"));
	TrailSpline->SetupAttachment(GetRootComponent());

	ParticleState = EGreenParticleState::EPS_Idle;
	ParticleType = EParticleType::EPT_Green;
	
}

void ARaphaelParticleGreen::SetThrowForce(float Force)
{
	ThrowForce = Force;
}

void ARaphaelParticleGreen::ClearLastTrail()
{
	TrailSpline->ClearSplinePoints();
	for(USplineMeshComponent* SplineMeshComponent : SplineMeshes)
	{
		SplineMeshComponent->DestroyComponent();
	}
	SplineMeshes.Empty();
}

void ARaphaelParticleGreen::SetTrailPath(TArray<FVector> Positions)
{
	const int Num = Positions.Num();
	TrailSpline->SetSplinePoints(Positions, ESplineCoordinateSpace::World);
	TrailSpline->SetSplinePointType(Num - 1, ESplinePointType::CurveClamped);

	// Spawn meshes
	for(int i = 0; i <= Num - 2; ++i)
	{
		USplineMeshComponent* SplineMesh = CreateSplineMesh();
		if(SplineMesh)
		{
			const FVector StartPosition = TrailSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			const FVector StartTangent = TrailSpline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
			const FVector EndPosition = TrailSpline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
			const FVector EndTangent = TrailSpline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
			SplineMeshes.Add(SplineMesh);
			SplineMesh->SetStartScale(FVector2D(.1, .1), true);
			SplineMesh->SetEndScale(FVector2D(.1, .1), true);
			SplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
		}
	}
	
}

void ARaphaelParticleGreen::CalculateInitVelocity()
{
	if(PlayerCharacter)
	{
		const FVector Direction = PlayerCharacter->GetFollowCamera()->GetForwardVector() + FVector(0, 0, 0.5f);
		InitVelocity = Direction * ThrowForce * 1000.f; 
	}
}

void ARaphaelParticleGreen::UnstableCollapse(const FHitResult& ImpactResult)
{
	Collapse();
}

bool ARaphaelParticleGreen::CaptureActor()
{
	CapturedActors.Empty();
	
	TArray<AActor*> TempActors;
	ActionArea->GetOverlappingActors(TempActors);
	if(TempActors.Num() == 0)
	{
		return false;
	}
	for(AActor* Item : TempActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Item->GetName());
		if(Item->ActorHasTag(FName("CollapseItem")))
		{
			CapturedActors.Add(Item);	
		}
	}
	return CapturedActors.Num() != 0;
}

// May override in BP to achieve better effect
void ARaphaelParticleGreen::DestroyCapturedActors_Implementation()
{
	for(AActor* CapturedActor : CapturedActors)
	{
		if(CapturedActor)
		{
			CapturedActor->Destroy();
		}
	}
}

void ARaphaelParticleGreen::ParticleActive_Implementation()
{
	if(PlayerCharacter == nullptr)
	{
		GetPlayerCharacter();
	}
	if(PlayerCharacter)
	{
		if(ParticleState == EGreenParticleState::EPS_Idle)
		{
			BeginAiming();
		}
		else if(ParticleState == EGreenParticleState::EPS_Aiming)
		{
			bIsLoad = false;
			SetChosenReaction(false);
			OnParticlePendingActive();
			
			ParticleMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			ParticleMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
			
			ThrowParticle();
		}
	}
}

void ARaphaelParticleGreen::ParticleSetFunctionEnable_Implementation(bool Enabled)
{
	if(ParticleState == EGreenParticleState::EPS_Moving)
	{
		if(Enabled)
		{
			Collapse();
		}
	}
}

void ARaphaelParticleGreen::Collapse_Implementation()
{
	ParticleState = EGreenParticleState::EPS_Collapse;
	OnParticlePendingKill();

	ActionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActionArea->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	ActionArea->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	ActionArea->SetSphereRadius(300.f);

	ParticleMesh->SetSimulatePhysics(false);
	TrailMovement->SetActive(false, true);
	
	// Effect finish in bp
}

void ARaphaelParticleGreen::ThrowParticle_Implementation()
{
	ParticleState = EGreenParticleState::EPS_Moving;
	GetWorldTimerManager().ClearTimer(ShowTrailHandle);
	ClearLastTrail();
	
	TrailMovement->SetVelocityInLocalSpace(InitVelocity);
	TrailMovement->bShouldBounce = true;
	TrailMovement->SetActive(true, true);
	
	TrailMovement->OnProjectileStop.AddDynamic(this, &ARaphaelParticleGreen::UnstableCollapse);
}

void ARaphaelParticleGreen::BeginAiming_Implementation()
{
	ParticleState = EGreenParticleState::EPS_Aiming;

	ThrowForce = ThrowInitForce;
	
	// Start aiming
	GetWorldTimerManager().SetTimer(ShowTrailHandle, this, &ARaphaelParticleGreen::ShowTrail, UpdateTrailStepTime, true);
	
}
