// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaphaelParticle.generated.h"

UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaphaelParticle();

public:
	// The curve used for loading
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	class UCurveVector* LoadCurve;

	// The curve used for dropping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	UCurveVector* DropCurve;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DropItself();

};
