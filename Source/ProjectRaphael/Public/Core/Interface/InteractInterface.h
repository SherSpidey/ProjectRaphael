// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EMagnetPoleType.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTRAPHAEL_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Show)
	void SetInteractReaction(bool bShow);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Magnet)
	void SetMagnetPoleType(EMagnetPoleType NewPoleType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Magnet)
	EMagnetPoleType GetMagnetPoleType();
};
