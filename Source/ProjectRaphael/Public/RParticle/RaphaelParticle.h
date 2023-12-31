// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Core/Interface/InteractInterface.h"
#include "RaphaelParticle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParticleTerpFinish);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParticleDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParticleActive);

UENUM(BlueprintType)
enum class EParticleType: uint8
{
	EPT_Black UMETA(DisplayName = "Black"),
	EPT_Silver UMETA(DisplayName = "Silver"),
	EPT_White UMETA(DisplayName = "White"),
	EPT_Green UMETA(DisplayName = "Green"),
	EPT_BR UMETA(DisplayName = "BR"),
	
	EPT_MAX UMETA(DisplayName = "Default")
};

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

	UPROPERTY(BlueprintReadWrite)
	class UPayLoadComponent* PayLoadComponent;
	
	// Curve used to translate
	UPROPERTY(BlueprintReadOnly)
	class UCurveVector* TranslationCurve;
	
	UPROPERTY(BlueprintReadWrite)
	FVector TargetPosition;

	UPROPERTY(BlueprintReadWrite)
	FVector PayLoadOriginPosition;

	UPROPERTY(BlueprintReadOnly)
	float TranslatePlayBackTime;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TranslateTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	float ParticleRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	float LoadMaxDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	EParticleType ParticleType;

	UPROPERTY(BlueprintReadWrite, Category="Function")
	class ABaseCharacter* PlayerCharacter;

public:

	FOnParticleTerpFinish OnTranslateFinish;

	FOnParticleDeath OnParticleDeath;
	
	FOnParticleActive OnParticleActive;

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

	UFUNCTION(BlueprintCallable,Category=Function)
	void GetPlayerCharacter();

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Function")
	void OnParticlePendingKill();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Function")
	void OnParticlePendingActive();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Function")
	void ParticleDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Update Position
	FORCEINLINE void UpdatePosition(FVector Position, FVector Origin) { TargetPosition = Position; PayLoadOriginPosition = Origin;}

	FORCEINLINE void SetOwnerPayloadComponent(UPayLoadComponent* OwnerPayload) { PayLoadComponent = OwnerPayload;}

	FORCEINLINE EParticleType GetParticleType() const { return ParticleType;}

	FORCEINLINE float GetLoadMaxDistance() const { return LoadMaxDistance;}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropItself();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyCurveToPosition();

	UFUNCTION(BlueprintCallable)
	void SetChosenReaction(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void ParticleActive();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void ParticleSetFunctionEnable(bool Enabled);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void SpecialFunction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Show)
	void SetInteractReaction(bool bShow);
	virtual void SetInteractReaction_Implementation(bool bShow) override;

};
