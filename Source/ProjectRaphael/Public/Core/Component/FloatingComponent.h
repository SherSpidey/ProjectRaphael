// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAPHAEL_API UFloatingComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UFloatingComponent();

protected:
	bool bSpawned;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	AActor* OwnerActor;
	
	UPROPERTY(BlueprintReadWrite, Category=Function)
	AActor* FloatingActor;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	FVector LandLocation;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	FTimerHandle SpawnTimerHandle;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	float FloatingHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	float ForceScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	float LoseControlRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	TSubclassOf<AActor> SpawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	int SpawnCount;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitOwner();

	void ResetSpawn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void ApplyForce();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void SetLandPosition(FVector Location);
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void SpawnSetParticle();

		
};
