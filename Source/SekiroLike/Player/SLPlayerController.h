// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASLPlayerController();

public:
	virtual void BeginPlay() override;

// HUD section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class USLHUDWidget> SLHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class USLHUDWidget> SLHUDWidget;

};
