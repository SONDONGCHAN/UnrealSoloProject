// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLCharacterItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLCharacterItemInterface : public UInterface
{
	GENERATED_BODY()
};


class SEKIROLIKE_API ISLCharacterItemInterface
{
	GENERATED_BODY()

public:
	virtual void TakeItem(class USLItemData* InItemData) = 0;
};
