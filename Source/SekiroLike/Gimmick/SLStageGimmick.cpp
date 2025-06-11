// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/SLStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/SLCollision.h"


// Sets default values
ASLStageGimmick::ASLStageGimmick()
{
	// Gate Section
	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate"));
	RootComponent = Gate;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Sample/Environment/Props/SM_GATE.SM_GATE'"));
	if (GateMeshRef.Object)
	{
		Gate->SetStaticMesh(GateMeshRef.Object);
	}

	GateTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("GateTigger"));
	GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
	GateTrigger->SetupAttachment(Gate);
	GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASLStageGimmick::OnGateTriggerBeginOverlap);
	GateTrigger->ComponentTags.Add(TEXT("GateTigger"));
}

void ASLStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Gate Trigger On"));
}

void ASLStageGimmick::OpenGate()
{
	GateTrigger->SetCollisionProfileName(CPROFILE_SLTRIGGER);
}
