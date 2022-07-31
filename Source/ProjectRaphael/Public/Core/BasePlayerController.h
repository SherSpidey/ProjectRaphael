// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAPHAEL_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setting)
	TSubclassOf<UUserWidget> MainUI;

protected:
	
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* HUD;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetHUDVisibility(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetCrossHairDynamic(float Scale);
	
};
