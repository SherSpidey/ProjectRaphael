// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBaseAnimInstance::UBaseAnimInstance()
{
}

void UBaseAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(OwnerCharacter == nullptr)
	{
		TryGetCharacter();
	}
	if(OwnerCharacter == nullptr)
	{
		// Still can not get the character, return
		return ;
	}
	// Update state
	CharacterState = OwnerCharacter->GetCharacterState();
	CharacterFunction = OwnerCharacter->GetCharacterFunction();
	
	CameraDeltaRotation = OwnerCharacter->GetFollowCameraRotation() - OwnerCharacter->GetActorRotation();
	
	const FVector Velocity = OwnerCharacter->GetVelocity();
	Speed = FVector(Velocity.X, Velocity.Y, 0).Size();

	bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
	Lean(DeltaTime);
}

void UBaseAnimInstance::NativeInitializeAnimation()
{
	TryGetCharacter();
}

void UBaseAnimInstance::Lean(float DeltaTime)
{
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation= OwnerCharacter->GetActorRotation();

	const FRotator Delta {UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame)};
	
	const float Target {Delta.Yaw};

	const float Interp {FMath::FInterpTo(YawDelta, Target, DeltaTime, 1.f)};

	YawDelta = FMath::Clamp(Interp, - 1.f, 1.f);
}

void UBaseAnimInstance::TryGetCharacter()
{
	// Get character
	APawn* Owner = TryGetPawnOwner();
	if(Owner == nullptr)
	{
		return ;
	}
	ABaseCharacter* OwnerPawn = Cast<ABaseCharacter>(Owner);
	if(OwnerPawn != nullptr)
	{
		OwnerCharacter = OwnerPawn;
	}
}
