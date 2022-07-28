// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_Walk UMETA(DisplayName = "Walk"),
	ECS_Run UMETA(DisplayName = "Run"),
	ECS_Sprint UMETA(DisplayName = "Sprint"),
	
	EPS_MAX UMETA(DisplayName = "Default")
};

UCLASS()
class PROJECTRAPHAEL_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Moving)
	float WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Moving)
	float RunningSpeed;

	// Sprint Speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Moving)
	float SprintSpeed;

private:
	bool bLockMovement;

	int OverlappedParticleCount;

	bool bShouldTraceForItems;

	UPROPERTY(BlueprintReadOnly, Category=Interact, meta = (AllowPrivateAccess = "true"))
	AActor* TraceHitItem;

	UPROPERTY(BlueprintReadOnly, Category=Interact, meta = (AllowPrivateAccess = "true"))
	AActor* TraceHitLastItem;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** User Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Climb, meta = (AllowPrivateAccess = "true"))
	class UPayLoadComponent* PayloadComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Climb, meta = (AllowPrivateAccess = "true"))
	class UClimbAndVaultComponent* ClimbComponent;
	
	UPROPERTY(BlueprintReadWrite, Category="State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState;

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	UFUNCTION(BlueprintCallable, Category=State)
	void UpdateStateProperties();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Moving)
	void StartJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Moving)
	void StopJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Moving)
	void StartSprinting();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Moving)
	void StopSprinting();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Climb)
	void Climb();

	UFUNCTION(BlueprintCallable, Category=Interact)
	bool TraceUnderCrossHairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	UFUNCTION(BlueprintCallable, Category=Interact)
	void TraceForItem();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Interact)
	void InteractWithItem();

	// When player has items, chose the next item
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	void ChoseNextItem();

	// When player has items, chose the previous item
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	void ChosePreviousItem();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns CameraBoom sub-object **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera sub-object **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE ECharacterState GetCharacterState() const {	return CharacterState; }

	FORCEINLINE void SetMovementEnable(bool Enable) { bLockMovement = !Enable; }

	UFUNCTION(BlueprintCallable, Category=Interact)
	void UpdateInteractNum(int Amount);

};
