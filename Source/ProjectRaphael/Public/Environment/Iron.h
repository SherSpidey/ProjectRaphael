// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interface/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "Iron.generated.h"

UCLASS()
class PROJECTRAPHAEL_API AIron : public AActor, public IInteractInterface 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIron();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	UStaticMeshComponent* IronMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
