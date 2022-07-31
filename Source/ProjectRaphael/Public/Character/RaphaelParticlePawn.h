// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RaphaelParticlePawn.generated.h"

UCLASS()
class PROJECTRAPHAEL_API ARaphaelParticlePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARaphaelParticlePawn();

	UPROPERTY(BlueprintReadWrite, Category=Setting)
	class ARaphaelParticle* ControlledParticle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USceneComponent* CameraRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Control)
	void ReleaseControl();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Control)
	void FunctionActivate();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Control)
	void FunctionDeactivate();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Set Controlled Particle
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Setting)
	void SetControlParticle(ARaphaelParticle* Particle);

};
