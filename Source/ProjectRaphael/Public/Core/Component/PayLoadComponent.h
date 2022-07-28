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
	// Make sure one update only update distribute once
	bool bAdjusted;
	
	// mark origin radius
	float OriginRadius;
	
	// mark when radius changed to trigger function
	float LastPayloadRadius;

	// copy last Location to update particle position
	FVector ComponentLastPosition;

	// copy last rotation to update particle position
	FRotator ComponentLastRotation;


protected:
	// The current chosen particle
	int CurrentIdx;
	
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

	// Limit when yaw change to update payloads
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	float MaxRotationUpdateLimit;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Call when particle load finish
	UFUNCTION(BlueprintCallable)
	void ParticleLoadFinish();

	// Drop particle after load particle with full payload.
	UFUNCTION(BlueprintCallable)
	void DropParticle(int DropId);

	// Arrange with new distribution
	// Called when changing radius and payload num.
	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateDistribution();

	//Control payload particles
	UFUNCTION(BlueprintCallable, Category="Update")
	void UpdateParticles();

	UFUNCTION(BlueprintCallable, Category="Update")
	void SetPickedParticle(int Index, bool ReSet);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	int GetPayLoadNum() {return PayLoads.Num();}

	UFUNCTION(BlueprintCallable)
	void LoadParticle(ARaphaelParticle* Particle);

	UFUNCTION(BlueprintCallable, Category="Update")
	void PickNextParticle();

	UFUNCTION(BlueprintCallable, Category="Update")
	void PickPreviousParticle();

	UFUNCTION(BlueprintCallable, Category="Update")
	void ActiveCurrentParticle();

	UFUNCTION(BlueprintCallable, Category="Update")
	ARaphaelParticle* GetCurrentParticle() { return PayLoads[CurrentIdx].Particle; };

	FORCEINLINE void SetMaxNum(int Num) { MaxParticleNum = Num; }

	FORCEINLINE void SetPayloadRadius(float Radius) { PayLoadRadius = Radius; }

		
};
