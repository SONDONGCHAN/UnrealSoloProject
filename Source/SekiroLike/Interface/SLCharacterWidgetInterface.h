// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLCharacterWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLCharacterWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SEKIROLIKE_API ISLCharacterWidgetInterface
{
	GENERATED_BODY()


public:
	virtual void SetupCharacterWidget(class USLUserWidget* InUserWidget) = 0;

};
