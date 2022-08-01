// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RParticle/RaphaelParticle.h"
#include "RaphaelParticleGreen.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticleGreen : public ARaphaelParticle
{
	GENERATED_BODY()

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	virtual void ParticleActive_Implementation() override;
};
