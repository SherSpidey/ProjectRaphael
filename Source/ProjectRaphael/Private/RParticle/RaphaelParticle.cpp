// Fill out your copyright notice in the Description page of Project Settings.


#include "RParticle/RaphaelParticle.h"

#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARaphaelParticle::ARaphaelParticle():
TranslateStepTime(0.001),
FollowPayloadSpeed(6.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParticleMesh"));
	SetRootComponent(ParticleMesh);

	ActionArea = CreateDefaultSubobject<USphereComponent>(TEXT("ActionArea"));
	ActionArea->SetupAttachment(ParticleMesh);
	
	ActionArea->SetCollisionResponseToChannels(ECR_Ignore);
	ActionArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ParticleMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

// Called when the game starts or when spawned
void ARaphaelParticle::BeginPlay()
{
	Super::BeginPlay();

	bIsLoad = false;

	// Set up overlap for AreaSphere
	ActionArea->OnComponentBeginOverlap.AddDynamic(this, &ARaphaelParticle::OnSphereBeginOverlap);
	ActionArea->OnComponentEndOverlap.AddDynamic(this, &ARaphaelParticle::OnSphereEndOverlap);

	ParticleMesh->SetSimulatePhysics(true);
}

void ARaphaelParticle::TranslateUpdate()
{
	TranslatePlayBackTime += TranslateStepTime;
	if(TranslatePlayBackTime >= TranslateDuration)
	{
		GetWorldTimerManager().ClearTimer(TranslateTimerHandle);
		bIsLoad = true;
		OnTranslateFinish.Broadcast();
		// change mesh setting
		// Avoid block pick
		ParticleMesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
		ActionArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Apply curve translation to particle
	const FVector TranslateAlpha = TranslationCurve->GetVectorValue(TranslatePlayBackTime);
	const FVector DeltaHeight = FVector(0, 0, TargetPosition.Z - TranslateStartPosition.Z);
	const FVector DeltaToOrigin = FVector(PayLoadOriginPosition.X - TranslateStartPosition.X, PayLoadOriginPosition.Y - TranslateStartPosition.Y, 0);
	const FVector DeltaOriginToTarget = FVector(TargetPosition.X - PayLoadOriginPosition.X, TargetPosition.Y - PayLoadOriginPosition.Y, 0);

	SetActorLocation(TranslateStartPosition + DeltaToOrigin * TranslateAlpha.X + DeltaOriginToTarget * TranslateAlpha.Y + DeltaHeight * TranslateAlpha.Z);
	//UKismetSystemLibrary::DrawDebugSphere(this , GetActorLocation(), 10, 12, FLinearColor::Red, 20, 1);
}

void ARaphaelParticle::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if(OtherActor)
	{
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
		if(PlayerCharacter)
		{
			PlayerCharacter->UpdateInteractNum(1);
		}
	}
}

void ARaphaelParticle::OnSphereEndOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
		if(PlayerCharacter)
		{
			PlayerCharacter->UpdateInteractNum(-1);
		}
	}
}

// Called every frame
void ARaphaelParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsLoad)
	{
		const FVector LerpPosition = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetPosition, DeltaTime, FollowPayloadSpeed);
		SetActorLocation(LerpPosition);
	}
}

void ARaphaelParticle::SetChosenReaction(bool Enable)
{
	ParticleMesh->SetRenderCustomDepth(Enable);
}

void ARaphaelParticle::ParticleActive_Implementation()
{
	// override in child class
	SetChosenReaction(false);
	DropItself();
}

void ARaphaelParticle::SetInteractReaction_Implementation(bool bShow)
{
	// may be do something later
	//SetChosenReaction(bShow);
}

// Apply Drop translation, maybe not.
void ARaphaelParticle::DropItself_Implementation()
{
	bIsLoad = false;
	// Set back
	ParticleMesh->SetSimulatePhysics(true);
	ParticleMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	ParticleMesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	ActionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Apply load translation
void ARaphaelParticle::ApplyCurveToPosition_Implementation()
{
	const UWorld* World = GetWorld();
	if(World)
	{
		float StartPoint;
		float EndPoint;
		LoadCurve->GetTimeRange(StartPoint, EndPoint);
		TranslateDuration = EndPoint - StartPoint;
		TranslationCurve = LoadCurve;
		TranslatePlayBackTime = 0.f;
		TranslateStartPosition = GetActorLocation();
		World->GetTimerManager().SetTimer(TranslateTimerHandle, this, &ARaphaelParticle::TranslateUpdate, TranslateStepTime, true);
		//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, FString::Printf(TEXT("Load, %f"), TranslateDuration));
	}
	// change mesh setting
	ParticleMesh->SetSimulatePhysics(false);
	ParticleMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
}

