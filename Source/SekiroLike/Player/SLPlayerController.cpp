// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SLPlayerController.h"
#include "UI/SLHUDWidget.h"

ASLPlayerController::ASLPlayerController()
{
	static ConstructorHelpers::FClassFinder<USLHUDWidget> SLHUDWidgetRef(TEXT("/Game/SekiroLike/UI/WBP_SLHUD.WBP_SLHUD_C"));
	if (SLHUDWidgetRef.Class)
	{
		SLHUDWidgetClass = SLHUDWidgetRef.Class;
	}
}

void ASLPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void ASLPlayerController::GameClear()
{
	K2_OnGameClear();
}

void ASLPlayerController::GameOver()
{
	K2_OnGameOver();
}

void ASLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	SLHUDWidget = CreateWidget<USLHUDWidget>(this, SLHUDWidgetClass);
	if (SLHUDWidget)
	{
		SLHUDWidget->AddToViewport();

	}
}
