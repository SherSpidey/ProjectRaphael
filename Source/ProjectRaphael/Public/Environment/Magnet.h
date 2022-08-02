// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interface/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "Magnet.generated.h"

UENUM(BlueprintType)
enum class EMagnetType: uint8
{
	EMT_Normal UMETA(DisplayName = "Normal"),
	EMT_Fixed UMETA(DisplayName = "Fixed"),
	EMT_Floating UMETA(DisplayName = "Floating"),
	
	EMT_MAX UMETA(DisplayName = "Default")
};

UCLASS()
class PROJECTRAPHAEL_API AMagnet : public AActor, public IInteractInterface 
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

protected:
	
	UPROPERTY(BlueprintReadWrite, Category=Force)
	TArray<AActor*> InfluencedActors;

	UPROPERTY(BlueprintReadWrite, Category=Force)
	float MaxInfluenceDistance;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	EMagnetPoleType PoleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	EMagnetType MagnetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting)
	float MaxForce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetMagnetSetting();

	float CalculateForce(const FVector& Start, const FVector& End) const;

	UFUNCTION(BlueprintCallable, Category=Force)
	void ApplyForce();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* GetMagnetMesh() const { return MagnetMesh; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Magnet)
	void SetMagnetPoleType(EMagnetPoleType NewPoleType);
	virtual void SetMagnetPoleType_Implementation(EMagnetPoleType NewPoleType) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Magnet)
	EMagnetPoleType GetMagnetPoleType();
	virtual EMagnetPoleType GetMagnetPoleType_Implementation() override;

};
