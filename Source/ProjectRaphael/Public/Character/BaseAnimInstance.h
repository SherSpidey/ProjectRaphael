// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/BaseCharacter.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UBaseAnimInstance();

	virtual  void NativeInitializeAnimation() override;

private:

	UPROPERTY(BlueprintReadOnly, Category=Movment,
		meta=(AllowPrivateAccess = "ture"))
	ABaseCharacter* OwnerCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category=Movment,
		meta=(AllowPrivateAccess = "ture"))
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category=Function,
		meta=(AllowPrivateAccess = "ture"))
	ECharacterFunction CharacterFunction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,
		meta=(AllowPrivateAccess = "ture"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,
		meta=(AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,
		meta=(AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,
		meta=(AllowPrivateAccess = "true"))
	FRotator CameraDeltaRotation;

	// Character Yaw this frame
	FRotator CharacterRotation;

	// Character Yaw last frame
	FRotator CharacterRotationLastFrame;

	// Yaw delta used for leaning in the running
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Lean",
		meta=(AllowPrivateAccess=true))
	float YawDelta;

public:
	// Update anime state
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

protected:

	// Handle calculation for leaning while running
	void Lean(float DeltaTime);

	// Get character
	void TryGetCharacter();

	
};
