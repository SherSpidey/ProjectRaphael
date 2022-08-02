// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interface/InteractInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_Walk UMETA(DisplayName = "Walk"),
	ECS_Run UMETA(DisplayName = "Run"),
	ECS_Sprint UMETA(DisplayName = "Sprint"),
	
	ECS_MAX UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class ECharacterFunction: uint8
{
	ECF_Idle UMETA(DisplayName = "Idle"),
	ECF_Control UMETA(DisplayName = "Control"),
	
	ECF_MAX UMETA(DisplayName = "Default")
};

UCLASS()
class PROJECTRAPHAEL_API ABaseCharacter : public ACharacter, public IInteractInterface
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

	bool bUsedParticleActivated;

	UPROPERTY(BlueprintReadOnly, Category=Controller, meta = (AllowPrivateAccess = "true"))
	AController* AIController;

	UPROPERTY(BlueprintReadOnly, Category=Interact, meta = (AllowPrivateAccess = "true"))
	AActor* TraceHitItem;

	UPROPERTY(BlueprintReadOnly, Category=Interact, meta = (AllowPrivateAccess = "true"))
	AActor* TraceHitLastItem;

	UPROPERTY(BlueprintReadOnly, Category=Interact, meta = (AllowPrivateAccess = "true"))
	class ARaphaelParticle* ChosenParticle;

	UPROPERTY(BlueprintReadOnly, Category=Interact, meta = (AllowPrivateAccess = "true"))
	ARaphaelParticle* UsedParticle;
	
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

	UPROPERTY(BlueprintReadWrite, Category="State", meta = (AllowPrivateAccess = "true"))
	ECharacterFunction CharacterFunction;

	UPROPERTY(BlueprintReadOnly, Category="Magnet", meta = (AllowPrivateAccess = "true"))
	EMagnetPoleType PlayerMagnetPoleType;

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

	void InteractSpecialCheck();

	UFUNCTION()
	void OnUsedParticleActive();
	
	UFUNCTION()
	void OnUsedParticleDestroyed();

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

	// When player chose a items, use it
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	void UseChosenItem();

	// When player chose a items, drop it
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	void DropChosenItem();

	// When player chose a items, hold
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	void ChosenItemHold();

	// When player use a items, release
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Item)
	void ChosenItemRelease();

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

	FORCEINLINE ECharacterFunction GetCharacterFunction() const {	return CharacterFunction; }

	FORCEINLINE void SetMovementEnable(bool Enable) { bLockMovement = !Enable; }

	FRotator GetFollowCameraRotation() const;

	UFUNCTION(BlueprintCallable, Category=Interact)
	void SetChosenParticle(ARaphaelParticle* Particle);

	UFUNCTION(BlueprintCallable, Category=Interact)
	void UpdateInteractNum(int Amount);

	UFUNCTION(BlueprintCallable, Category=Interact)
	AActor* TraceForObjectOnce();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Interact)
	AController* SetAIController();

	UFUNCTION(BlueprintCallable, Category=Interact)
	AController* GetAIController() { return AIController;}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Magnet)
	void SetMagnetPoleType(EMagnetPoleType NewPoleType);
	virtual void SetMagnetPoleType_Implementation(EMagnetPoleType NewPoleType) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Magnet)
	EMagnetPoleType GetMagnetPoleType();
	virtual EMagnetPoleType GetMagnetPoleType_Implementation() override;
};
