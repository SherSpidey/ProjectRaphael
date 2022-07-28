// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Core/Interface/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "RaphaelParticle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParticleTerpFinish);

UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticle : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaphaelParticle();

protected:
	bool bIsLoad;

	float TranslateDuration;

	FVector TranslateStartPosition;

	UPROPERTY()
	class UPayLoadComponent* PayLoadComponent;

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

	// Interact
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepHitResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Update Position
	FORCEINLINE void UpdatePosition(FVector Position, FVector Origin) { TargetPosition = Position; PayLoadOriginPosition = Origin;}

	FORCEINLINE void SetOwnerPayloadComponent(UPayLoadComponent* OwnerPayload) { PayLoadComponent = OwnerPayload;}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropItself();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyCurveToPosition();

	UFUNCTION(BlueprintCallable)
	void SetChosenReaction(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void ParticleActive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Show)
	void SetInteractReaction(bool bShow);
	virtual void SetInteractReaction_Implementation(bool bShow) override;

};
