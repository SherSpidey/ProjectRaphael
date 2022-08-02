// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Magnet.h"

#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMagnet::AMagnet():
MaxForce(500000.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MagnetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagnetMesh"));
	SetRootComponent(MagnetMesh);

	AffectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AffectionSphere"));
	AffectionSphere->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AMagnet::BeginPlay()
{
	Super::BeginPlay();
	
	SetMagnetSetting();
	
}

void AMagnet::SetMagnetSetting()
{
	MaxInfluenceDistance = AffectionSphere->GetScaledSphereRadius();
	switch(MagnetType)
	{
		case EMagnetType::EMT_Normal:
			MagnetMesh->SetSimulatePhysics(true);
			break;
		case EMagnetType::EMT_Fixed:
			MagnetMesh->SetSimulatePhysics(false);
			break;
		case EMagnetType::EMT_Floating:
			MagnetMesh->SetSimulatePhysics(true);
			MagnetMesh->SetEnableGravity(false);
			break;
		case EMagnetType::EMT_MAX: break;
		default: ;
	}
}

float AMagnet::CalculateForce(const FVector& Start, const FVector& End) const
{
	const float Distance = FMath::Clamp<float>((Start - End).Size(), 0, MaxInfluenceDistance) ;

	const float ForceValue =  MaxForce * FMath::Pow((MaxInfluenceDistance - Distance) / MaxInfluenceDistance, 3);
	
	return ForceValue;
}

void AMagnet::ApplyForce()
{
	
	TArray<AActor*> TempActors;
	AffectionSphere->GetOverlappingActors(TempActors);
	if(TempActors.Num() == 0)
	{
		return ;
	}
	
	FVector SelfPosition = MagnetMesh->GetCenterOfMass();
	
	for(AActor* OtherActor : TempActors)
	{
		// Skip self
		if(OtherActor == this)
		{
			continue;
		}
		
		const IInteractInterface* MagnetInterface = Cast<IInteractInterface>(OtherActor);
		if(MagnetInterface)
		{
			const EMagnetPoleType TargetPoleType = MagnetInterface->Execute_GetMagnetPoleType(OtherActor);
			// other actor can be influenced!
			if(TargetPoleType != EMagnetPoleType::EMT_MAX && TargetPoleType != EMagnetPoleType::EMT_Nonmagnetic)
			{
				// Check if other actor is a magnet
				const AMagnet* OtherMagnet = Cast<AMagnet>(OtherActor);
				if(OtherMagnet)
				{
					// You can not influence fixed magnet
					if(OtherMagnet->MagnetType == EMagnetType::EMT_Fixed)
					{
						continue;
					}
					UStaticMeshComponent* OtherMagnetMesh = OtherMagnet->GetMagnetMesh();
					FVector TargetPosition = OtherMagnetMesh->GetCenterOfMass();
					if(MagnetType == EMagnetType::EMT_Fixed)
					{
						MagnetMesh->GetClosestPointOnCollision(TargetPosition, SelfPosition);
					}
					const float ForceValue = CalculateForce(SelfPosition, TargetPosition);
					const float Direction = (TargetPoleType != PoleType) ? 1.f : -1.f;
					
					const FVector Force = (SelfPosition - TargetPosition).GetSafeNormal() * ForceValue * Direction * 0.5;

					// Apply Force to Magnet
					OtherMagnetMesh->AddForceAtLocation(Force, TargetPosition);

					if(MagnetType != EMagnetType::EMT_Fixed)
					{
						MagnetMesh->AddForceAtLocation(-1 * Force, SelfPosition);
					}
					//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%f"), ForceValue));
					continue;
				}
				
				// Check if other actor is player
				const ABaseCharacter* PlayerCharacter =  Cast<ABaseCharacter>(OtherActor);
				if(PlayerCharacter)
				{
					UCharacterMovementComponent* PlayerMovement = PlayerCharacter->GetCharacterMovement();
					FVector TargetPosition = PlayerCharacter->GetActorLocation();
					if(MagnetType == EMagnetType::EMT_Fixed)
					{
						MagnetMesh->GetClosestPointOnCollision(TargetPosition, SelfPosition);
					}
					const float ForceValue = CalculateForce(SelfPosition, TargetPosition);
					const float Direction = (TargetPoleType != PoleType) ? 1.f : -1.f;

					const FVector Force = (SelfPosition - TargetPosition).GetSafeNormal() * ForceValue * Direction * 0.5;

					// Apply Force to Player
					PlayerMovement->AddForce(Force);

					if(MagnetType != EMagnetType::EMT_Fixed)
					{
						MagnetMesh->AddForceAtLocation(-1 * Force, SelfPosition);
					}
					continue;
				}

				// Actor is Iron
				if(PoleType == EMagnetPoleType::EMT_Iron)
				{
					UActorComponent* ActorComponent = OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
					if(ActorComponent)
					{
						UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponent);
						FVector TargetPosition = StaticMeshComponent->GetCenterOfMass();
						if(MagnetType == EMagnetType::EMT_Fixed)
						{
							MagnetMesh->GetClosestPointOnCollision(TargetPosition, SelfPosition);
						}
						const float ForceValue = CalculateForce(SelfPosition, TargetPosition);
						const float Direction = (TargetPoleType != PoleType) ? 1.f : -1.f;
					
						const FVector Force = (SelfPosition - TargetPosition).GetSafeNormal() * ForceValue * Direction * 0.5;
						
						// Apply Force to Magnet
						StaticMeshComponent->AddForceAtLocation(Force, TargetPosition);

						if(MagnetType != EMagnetType::EMT_Fixed)
						{
							MagnetMesh->AddForceAtLocation(-1 * Force, SelfPosition);
						}
					}
				}
			}
		}
	}
}

// Called every frame
void AMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyForce();
}

void AMagnet::SetMagnetPoleType_Implementation(EMagnetPoleType NewPoleType)
{
	PoleType = NewPoleType;
}

EMagnetPoleType AMagnet::GetMagnetPoleType_Implementation()
{
	return PoleType;
}

