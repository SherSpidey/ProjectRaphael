// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magnet.generated.h"

UENUM(BlueprintType)
enum class EMagnetType: uint8
{
	EMT_Normal UMETA(DisplayName = "Normal"),
	EMT_Fixed UMETA(DisplayName = "Fixed"),
	EMT_Platform UMETA(DisplayName = "Platform"),
	
	EMT_MAX UMETA(DisplayName = "Default")
};

UCLASS()
class PROJECTRAPHAEL_API AMagnet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagnet();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	UStaticMeshComponent* MagnetMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	class USphereComponent* AffectionSphere;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Setting)
	EMagnetType MagnetType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetMagnetSetting();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
