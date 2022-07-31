// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RParticle/RaphaelParticle.h"
#include "RaphaelParticleSilver.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticleSilver : public ARaphaelParticle
{
	GENERATED_BODY()
	
	ARaphaelParticleSilver();

protected:

	UPROPERTY(BlueprintReadWrite, Category=Function)
	class ABasePlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	class ARaphaelParticlePawn* ControllerPawn;

	UFUNCTION(BlueprintCallable, Category=Function)
	void BackToPlayer();

public:
	virtual void ParticleActive_Implementation() override;
	
};
