// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EMagnetPoleType.h"
#include "RParticle/RaphaelParticle.h"
#include "RaphaelParticleBR.generated.h"

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

	UPROPERTY(BlueprintReadWrite, Category=Function)
	AActor* ControlledActor;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	FVector StartLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	class UNiagaraComponent* ControlParticle;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	EBRParticleState ParticleState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	EMagnetPoleType MagnetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	UMaterialInterface* NorthMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	UMaterialInterface* SouthMaterial;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category=Setting)
	void SetParticleMagnet();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void CheckMagnet();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Function)
	void BeginControl();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void ControlMagnet();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void DestroyEquippedParticle();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void SetControlParticleMaterial();

public:

	virtual void ParticleActive_Implementation() override;

	virtual void ParticleSetFunctionEnable_Implementation(bool Enabled) override;

	virtual void SpecialFunction_Implementation() override;

	UFUNCTION(BlueprintCallable, Category=Setting)
	EMagnetPoleType GetMagnetType() const { return MagnetType; }
	
};
