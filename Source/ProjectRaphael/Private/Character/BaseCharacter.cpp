// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Component/ClimbAndVaultComponent.h"
#include "Core/Component/PayLoadComponent.h"
//#include "Core/Interface/InteractInterface.h"
#include "Core/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RParticle/RaphaelParticle.h"

// Sets default values
ABaseCharacter::ABaseCharacter():
WalkSpeed(200),
RunningSpeed(400),
SprintSpeed(600),
bLockMovement(false),
OverlappedParticleCount(0),
bShouldTraceForItems(false),
bUsedParticleActivated(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 240.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PayloadComponent = CreateDefaultSubobject<UPayLoadComponent>(TEXT("PayLoad"));
	PayloadComponent->SetupAttachment(RootComponent);
	PayloadComponent->SetMaxNum(5);
	PayloadComponent->SetPayloadRadius(49.f);
	PayloadComponent->SetRelativeLocation(FVector(-55.f, 0, 50.f));
	
	// Add climb component
	ClimbComponent = CreateDefaultSubobject<UClimbAndVaultComponent>(TEXT("ClimbComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	PlayerMagnetPoleType = EMagnetPoleType::EMT_Nonmagnetic;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Custom setting
	CharacterState = ECharacterState::ECS_Idle;
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;

	AIController = SetAIController();
}

void ABaseCharacter::MoveForward(float Value)
{
	if(bLockMovement)
	{
		return ;
	}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if(bLockMovement)
	{
		return ;
	}
	
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ABaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ABaseCharacter::InteractSpecialCheck()
{
	if(ChosenParticle != nullptr)
	{
		const EParticleType ParticleType = ChosenParticle->GetParticleType();
		if(ParticleType == EParticleType::EPT_Black)
		{
			bShouldTraceForItems = true;
		}
		else
		{
			// resume normal
			UpdateInteractNum(0);
		}
	}
}

void ABaseCharacter::OnUsedParticleActive()
{
	if(UsedParticle && PayloadComponent)
	{
		bUsedParticleActivated = true;
		UsedParticle->OnParticleActive.RemoveAll(this);
		PayloadComponent->ActiveCurrentParticle();
		if(UsedParticle->GetParticleType() == EParticleType::EPT_Black)
		{
			CharacterFunction = ECharacterFunction::ECF_Control;
		}
	}
}

void ABaseCharacter::OnUsedParticleDestroyed()
{
	if(UsedParticle)
	{
		UsedParticle->OnParticleDeath.RemoveAll(this);
	}
	UsedParticle = nullptr;
	bUsedParticleActivated = false;
	CharacterFunction = ECharacterFunction::ECF_Idle;
}

void ABaseCharacter::UpdateStateProperties()
{
	// do nothing
}

void ABaseCharacter::StopJump_Implementation()
{
	
	StopJumping();
}

void ABaseCharacter::StartJump_Implementation()
{
	if(bLockMovement)
	{
		return ;
	}
	Jump();
}

//
void ABaseCharacter::StartSprinting_Implementation()
{
	if(bLockMovement)
	{
		return ;
	}
	
	if(CharacterState == ECharacterState::ECS_Idle || CharacterState == ECharacterState::ECS_Run ||
		CharacterState == ECharacterState::ECS_Walk)
	{
		CharacterState = ECharacterState::ECS_Sprint;
		// SetMaxSpeed
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABaseCharacter::StopSprinting_Implementation()
{
	if(CharacterState == ECharacterState::ECS_Sprint)
	{
		CharacterState = ECharacterState::ECS_Run;
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void ABaseCharacter::Climb_Implementation()
{
	if(ClimbComponent != nullptr)
	{
		ClimbComponent->Climb();
	}
}

FRotator ABaseCharacter::GetFollowCameraRotation() const
{
	return FollowCamera->GetComponentRotation();
}

void ABaseCharacter::SetChosenParticle(ARaphaelParticle* Particle)
{
	ChosenParticle = Particle;
	InteractSpecialCheck();
}

void ABaseCharacter::UpdateInteractNum(int Amount)
{
	if(OverlappedParticleCount + Amount <= 0)
	{
		OverlappedParticleCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedParticleCount += Amount;
		bShouldTraceForItems = true;
	}
	if(Amount != 0)
	{
		// Update Special condition
		InteractSpecialCheck();
	}
}

AActor* ABaseCharacter::TraceForObjectOnce()
{
	const bool bLastShouldTrace = bShouldTraceForItems;
	bShouldTraceForItems = true;
	TraceForItem();
	bShouldTraceForItems = bLastShouldTrace;
	return TraceHitItem;
}

void ABaseCharacter::SetMagnetPoleType_Implementation(EMagnetPoleType NewPoleType)
{
	PlayerMagnetPoleType = NewPoleType;
}

EMagnetPoleType ABaseCharacter::GetMagnetPoleType_Implementation()
{
	return PlayerMagnetPoleType;
}

bool ABaseCharacter::TraceUnderCrossHairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	// Get viewport size
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	// Get world position and direction of cross hair
	const FVector2D CrossHairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;
	const bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
																		 CrossHairLocation,
																		 CrossHairWorldPosition,
																		 CrossHairWorldDirection);

	if(bScreenToWorld)
	{
		const FVector Start {CrossHairWorldPosition};
		const FVector End {Start + CrossHairWorldDirection * 50'000.f};
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);
		if(OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void ABaseCharacter::TraceForItem()
{
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(Controller);
	if(bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector ItemLocation;
		TraceUnderCrossHairs(ItemTraceResult, ItemLocation);
		if(ItemTraceResult.bBlockingHit)
		{
			TraceHitItem = ItemTraceResult.GetActor();
			const IInteractInterface* ItemInterface = Cast<IInteractInterface>(TraceHitItem);
			if(ItemInterface)
			{
				if(PlayerController)
				{
					PlayerController->SetCrossHairDynamic(20.f);
				}
				// Show Item's OutLine
				ItemInterface->Execute_SetInteractReaction(TraceHitItem, true);
			
				if(TraceHitLastItem)
				{
					if(TraceHitLastItem != TraceHitItem)
					{
						const IInteractInterface* LastItemInterface = Cast<IInteractInterface>(TraceHitItem);
						if(LastItemInterface)
						{
							LastItemInterface->Execute_SetInteractReaction(TraceHitItem, false);
						}
					}
				}
				TraceHitLastItem = TraceHitItem;
			}
			else
			{
				if(PlayerController)
				{
					PlayerController->SetCrossHairDynamic(0.f);
				}
			}
		}
		return ;
	}
	else if(TraceHitLastItem)
	{
		if(PlayerController)
		{
			PlayerController->SetCrossHairDynamic(0.f);
		}
		
		const IInteractInterface* LastItemInterface = Cast<IInteractInterface>(TraceHitItem);
		if(LastItemInterface)
		{
			LastItemInterface->Execute_SetInteractReaction(TraceHitItem, false);
		}
		TraceHitLastItem = nullptr;
	}
	TraceHitItem = nullptr;
}

void ABaseCharacter::InteractWithItem_Implementation()
{
	if(TraceHitItem)
	{
		ARaphaelParticle* Particle = Cast<ARaphaelParticle>(TraceHitItem);
		if(Particle)
		{
			const float Distance = (GetActorLocation() - Particle->GetActorLocation()).Size();
			if(Distance <= Particle->GetLoadMaxDistance() && PayloadComponent)
			{
				PayloadComponent->LoadParticle(Particle);
			}
		}
		TraceHitItem = nullptr;
	}
}

void ABaseCharacter::ChoseNextItem_Implementation()
{
	if(PayloadComponent)
	{
		PayloadComponent->PickNextParticle();
	}
}

void ABaseCharacter::ChosePreviousItem_Implementation()
{
	if(PayloadComponent)
	{
		PayloadComponent->PickPreviousParticle();
	}
}

void ABaseCharacter::UseChosenItem_Implementation()
{
	if(UsedParticle == nullptr || !bUsedParticleActivated)
	{
		UsedParticle = ChosenParticle;
		// Bind to remove particle
		if(UsedParticle)
		{
			UsedParticle->OnParticleActive.AddUniqueDynamic(this, &ABaseCharacter::OnUsedParticleActive);
			UsedParticle->OnParticleDeath.AddUniqueDynamic(this, &ABaseCharacter::OnUsedParticleDestroyed);
		}
	}
	if(UsedParticle)
	{
		UsedParticle->ParticleActive();
	}
	
}

void ABaseCharacter::ChosenItemHold_Implementation()
{
	if(UsedParticle)
	{
		UsedParticle->ParticleSetFunctionEnable(true);
	}
}

void ABaseCharacter::ChosenItemRelease_Implementation()
{
	if(UsedParticle)
	{
		UsedParticle->ParticleSetFunctionEnable(false);
	}
}

void ABaseCharacter::DropChosenItem_Implementation()
{
	if(PayloadComponent)
	{
		PayloadComponent->DropCurrentParticle();
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// line trace when have interactable items
	TraceForItem();

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABaseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABaseCharacter::TouchStopped);

	// Custom action binding
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Climb", IE_Pressed, this, &ABaseCharacter::Climb);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseCharacter::InteractWithItem);

	PlayerInputComponent->BindAction("ItemNext", IE_Pressed, this, &ABaseCharacter::ChoseNextItem);
	PlayerInputComponent->BindAction("ItemPrevious", IE_Pressed, this, &ABaseCharacter::ChosePreviousItem);

	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &ABaseCharacter::UseChosenItem);

	PlayerInputComponent->BindAction("ItemHold", IE_Pressed, this, &ABaseCharacter::ChosenItemHold);
	PlayerInputComponent->BindAction("ItemRelease", IE_Released, this, &ABaseCharacter::ChosenItemRelease);

	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &ABaseCharacter::DropChosenItem);

}

