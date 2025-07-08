// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SLGameMode.h"
#include "Player/SLPlayerController.h"


ASLGameMode::ASLGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/SekiroLike/Blueprint/BP_SLCharacterPlayer.BP_SLCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/SekiroLike.SLPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void ASLGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore = NewPlayerScore;

	ASLPlayerController* SLPlayerController = Cast<ASLPlayerController>(GetWorld()->GetFirstPlayerController());
	if (SLPlayerController)
	{
		SLPlayerController->GameScoreChanged(CurrentScore);
	}

	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;
		SLPlayerController->GameClear();
	}
}

void ASLGameMode::OnPlayerDead()
{
	ASLPlayerController* SLPlayerController = Cast<ASLPlayerController>(GetWorld()->GetFirstPlayerController());
	if (SLPlayerController)
	{
		SLPlayerController->GameOver();
	}
}

bool ASLGameMode::IsGameCleared()
{
	return bIsCleared;
}
