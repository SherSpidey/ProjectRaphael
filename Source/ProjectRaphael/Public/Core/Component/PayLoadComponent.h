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

private:
	// mark when radius changed to trigger function
	float LastPayloadRadius;

	// copy last Location to update particle position
	FVector ComponentLastPosition;

protected:
	
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

	//Control payload particles
	void UpdateParticles();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	int GetPayLoadNum() {return PayLoads.Num();}

	UFUNCTION(BlueprintCallable)
	void LoadParticle(ARaphaelParticle* Particle);

		
};
