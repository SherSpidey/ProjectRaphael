// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RParticle/RaphaelParticle.h"
#include "RaphaelParticleBR.generated.h"

UENUM(BlueprintType)
enum class EBRParticleType: uint8
{
	EPT_North UMETA(DisplayName = "North"),
	EPT_South UMETA(DisplayName = "South"),
	
	EPT_MAX UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EBRParticleState: uint8
{
	EPS_Idle UMETA(DisplayName = "Idle"),
	EPS_Function UMETA(DisplayName = "Function"),
	EPS_Control UMETA(DisplayName = "Control"),
	EPS_Equipped UMETA(DisplayName = "Equipped"),
	
	EPS_MAX UMETA(DisplayName = "Default")
};

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticleBR : public ARaphaelParticle
{
	GENERATED_BODY()

	ARaphaelParticleBR();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	EBRParticleState ParticleState;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	EBRParticleType MagnetType; 

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category=Setting)
	EBRParticleType GetMagnetType() const { return MagnetType; } 
	
};
