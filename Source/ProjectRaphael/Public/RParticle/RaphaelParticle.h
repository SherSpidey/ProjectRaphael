// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaphaelParticle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnParticleTerpFinish, int, DorpID);

UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaphaelParticle();

protected:
	bool bIsLoad;

	float TranslateDuration;

	FVector TranslateStartPosition;

	// Curve used to translate
	UPROPERTY()
	class UCurveVector* TranslationCurve;
	
	UPROPERTY(BlueprintReadWrite)
	FVector TargetPosition;

	UPROPERTY(BlueprintReadWrite)
	FVector PayLoadOriginPosition;

	UPROPERTY(BlueprintReadOnly)
	float TranslatePlayBackTime;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TranslateTimerHandle;

public:

	FOnParticleTerpFinish OnTranslateFinish;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	float TranslateStepTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Setting")
	float FollowPayloadSpeed;
	
	// The curve used for loading
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	UCurveVector* LoadCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Default")
	UStaticMeshComponent* ParticleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Default")
	class USphereComponent* ActionArea;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TranslateUpdate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Update Position
	FORCEINLINE void UpdatePosition(FVector Position, FVector Origin) { TargetPosition = Position; PayLoadOriginPosition = Origin;}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropItself();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyCurveToPosition();

};
