// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PayLoadComponent.generated.h"

USTRUCT(BlueprintType)
struct FParticlePayloadInfo
{
	GENERATED_USTRUCT_BODY();

	// Particle reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class ARaphaelParticle* Particle;

	// Particle idle position
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Position;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAPHAEL_API UPayLoadComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPayLoadComponent();

protected:
	// The number of in load particles
	UPROPERTY(BlueprintReadWrite)
	int ParticleNum;
	
	// The particle idle position, update every frame
	UPROPERTY(BlueprintReadWrite)
	TArray<FParticlePayloadInfo> PayLoads;
	
	
public:
	// Max carry number
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	int MaxParticleNum;
		
	// The radius of the payload component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	float PayLoadRadius;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Drop particle after load particle with full payload.
	void DropParticle(int DropId);

	// Arrange with new distribution
	// Called when changing radius and payload num.
	void UpdateDistribution();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void LoadParticle(class ARaphaelParticle* Particle);

		
};
