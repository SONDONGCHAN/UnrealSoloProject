// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemField.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/SLCollision.h"
#include "Interface/SLCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "SLItemData.h"

ASLItemField::ASLItemField()
{
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_SLTRIGGER);
	Trigger->SetBoxExtent(FVector(13.f, 13.5f, 4.f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASLItemField::OnOverlapBegin);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/FirstAidKit/StaticMesh/SM_AidKitMerge.SM_AidKitMerge'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -42.3f, 1.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Sample/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}
}

void ASLItemField::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASLItemField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASLItemField::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	UAssetManager& Manager = UAssetManager::Get();
	
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("SLItemData"), Assets);
	ensure(0 < Assets.Num());
	
	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<USLItemData>(AssetPtr.Get());
	ensure(Item);
}

void ASLItemField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	ISLCharacterItemInterface* OverlappingPawn = Cast<ISLCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}
	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &ASLItemField::OnEffectFinished);
}

void ASLItemField::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

