// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RParticle/RaphaelParticle.h"
#include "RaphaelParticleBlack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticleBlack : public ARaphaelParticle
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Function")
	AActor* TargetActor;


	void GetTargetActor();
	
	// Override to achieve functional uses
	virtual void OnSphereBeginOverlap_Implementation(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepHitResult) override;

public:
	virtual void ParticleActive_Implementation() override;

	
	
};
