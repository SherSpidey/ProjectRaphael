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

	ARaphaelParticleBlack();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	class UNiagaraComponent* ControlParticle;
	
	UPROPERTY(BlueprintReadWrite, Category="Function")
	AActor* TargetActor;
	
	UPROPERTY(BlueprintReadWrite, Category="Function")
	FVector StartPosition;

	UPROPERTY(BlueprintReadWrite, Category="Function")
	bool bIsFunctional;

	UPROPERTY(BlueprintReadWrite, Category="Function")
	bool bControlled;

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

	virtual void ParticleSetFunctionEnable_Implementation(bool Enabled) override;
	
};
