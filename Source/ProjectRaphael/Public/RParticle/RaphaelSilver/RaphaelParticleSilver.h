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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Movement)
	float JumpInitPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Movement)
	float JumpMaxPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Movement)
	float MovingForce;

	UPROPERTY(BlueprintReadWrite, Category=Movement)
	float JumpPower;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	class ABasePlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	class ARaphaelParticlePawn* ControllerPawn;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void BackToPlayer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void JumpHold();

public:
	virtual void ParticleActive_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void Rolling(FVector Direction, float Scale);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Movement)
	void Jump(FVector Direction);
	
};
