// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RParticle/RaphaelParticle.h"
#include "RaphaelParticleBlack.generated.h"


UENUM(BlueprintType)
enum class EBlackParticleState: uint8
{
	EPS_Idle UMETA(DisplayName = "Idle"),
	EPS_Moving UMETA(DisplayName = "Moving"),
	EPS_Control UMETA(DisplayName = "Control"),
	
	EPS_MAX UMETA(DisplayName = "Default")
};

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticleBlack : public ARaphaelParticle
{
	GENERATED_BODY()

	ARaphaelParticleBlack();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	float ControlDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	float ControlPressDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	float PressForceScale;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Setting")
	float CurrentDistance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	class UNiagaraComponent* ControlParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	class UPhysicsHandleComponent* ControlHandle;
	
	UPROPERTY(BlueprintReadWrite, Category="Function")
	AActor* TargetActor;

	UPROPERTY(BlueprintReadWrite, Category="Function")
	class ABaseCharacter* PlayerCharacter;
	
	UPROPERTY(BlueprintReadWrite, Category="Function")
	FVector StartPosition;

	UPROPERTY(BlueprintReadWrite, Category="Function")
	EBlackParticleState ParticleState;

	UPROPERTY(BlueprintReadWrite, Category="Function")
	FVector InterpSpeed;

	UFUNCTION(BlueprintCallable, Category="Function")
	void CalculateSpeed(float InterpTime);

	UFUNCTION(BlueprintCallable, Category="Function")
	void GetTargetActor();

	UFUNCTION(BlueprintCallable, Category="Function")
	void Transformer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Function")
	void ControlParticleInit();

	UFUNCTION(BlueprintCallable, Category="Function")
	void ControlTarget();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Function")
	void ControlParticleDeath();

	UFUNCTION(BlueprintCallable, Category="Function")
	void OnControlParticleDeathFinish();

	UFUNCTION(BlueprintCallable, Category="Function")
	void ApplyPressForce();

public:
	virtual void ParticleActive_Implementation() override;

	virtual void ParticleSetFunctionEnable_Implementation(bool Enabled) override;
	
};
