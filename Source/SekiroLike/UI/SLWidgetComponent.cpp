// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLWidgetComponent.h"
#include "SLUserWidget.h"

void USLWidgetComponent::InitWidget()
{
	Super::InitWidget();

	USLUserWidget* SLUserWidget = Cast<USLUserWidget>(GetWidget());
	if (SLUserWidget)
	{
		SLUserWidget->SetOwingActor(GetOwner());
	}
}
