// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BasePlayerController.h"

#include "Blueprint/UserWidget.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(MainUI)
	{
		HUD = CreateWidget<UUserWidget>(this, MainUI);
		
		HUD->AddToViewport();
		HUD->SetVisibility(ESlateVisibility::Visible);
	}
}
