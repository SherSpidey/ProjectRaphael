// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MyBlueprintFunctionLibrary.h"

#include "GenericPlatform/GenericPlatformApplicationMisc.h"

FLinearColor UMyBlueprintFunctionLibrary::GetScreenColorFromPosition(FVector2D ScreenPosition)
{
	return FGenericPlatformApplicationMisc::GetScreenPixelColor(ScreenPosition);	
}
