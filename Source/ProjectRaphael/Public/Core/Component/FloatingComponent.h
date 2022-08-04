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

	UPROPERTY(BlueprintReadWrite, Category=Function)
	AActor* OwnerActor;
	
	UPROPERTY(BlueprintReadWrite, Category=Function)
	AActor* FloatingActor;

	UPROPERTY(BlueprintReadWrite, Category=Function)
	FVector LandLocation;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	float FloatingHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	float ForceScale;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitOwner();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Function)
	void ApplyForce();
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
