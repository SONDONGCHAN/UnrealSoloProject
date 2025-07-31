// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SLProjectilePoolManager.h"
#include "SLProjectile.h"
#include "EngineUtils.h"

ASLProjectilePoolManager* ASLProjectilePoolManager::MyInstance = nullptr;

ASLProjectilePoolManager::ASLProjectilePoolManager()
{

}

ASLProjectilePoolManager::~ASLProjectilePoolManager()
{
	for (auto& Pair : ProjectilePools)
	{
		delete Pair.Value;
	}
	ProjectilePools.Empty();
}

void ASLProjectilePoolManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto& Elem : ProjectileClasses)
	{
		const EProjectileType Type = Elem.Key;
		TSubclassOf<ASLProjectile> ProjClass = Elem.Value;

		TQueue<ASLProjectile*>* NewQueue = new TQueue<ASLProjectile*>();

		for (int32 i = 0; i < BasePoolSize; ++i)
		{
			ASLProjectile* Proj = GetWorld()->SpawnActor<ASLProjectile>(ProjClass);
			Proj->InitializeProjectile(Type);
			Proj->ResetProjectile();
			NewQueue->Enqueue(Proj);
		}

		ProjectilePools.Add(Type, NewQueue);
	}
}

ASLProjectilePoolManager* ASLProjectilePoolManager::GetInstance(UWorld* World)
{
	if (!MyInstance)
	{
		for (TActorIterator<ASLProjectilePoolManager> It(World); It; ++It)
		{
			MyInstance = *It;
			break;
		}
		if (!MyInstance)
		{
			MyInstance = World->SpawnActor<ASLProjectilePoolManager>();
		}
	}

	return MyInstance;
}

ASLProjectile* ASLProjectilePoolManager::GetProjectile(EProjectileType ProjectileType)
{
	TQueue<ASLProjectile*>** ProjectilePoolPtr = ProjectilePools.Find(ProjectileType);

	if (!ProjectilePoolPtr)
	{
		TQueue<ASLProjectile*>* NewQueue = new TQueue<ASLProjectile*>();
		ProjectilePools.Add(ProjectileType, NewQueue);
		ProjectilePoolPtr = ProjectilePools.Find(ProjectileType);
	}

	ASLProjectile* Projectile = nullptr;
	if(ProjectilePools[ProjectileType]->Dequeue(Projectile) && Projectile)
	{
		UE_LOG(LogTemp, Log, TEXT("UseProjectile : Use [%s] Projectile"), *UEnum::GetValueAsString(ProjectileType));
		return Projectile;
	}
	
	Projectile = GetWorld()->SpawnActor<ASLProjectile>(ProjectileClasses[ProjectileType]);
	Projectile->InitializeProjectile(ProjectileType);

	UE_LOG(LogTemp, Log, TEXT("UseProjectile : Make New [%s] Projectile Instance"),*UEnum::GetValueAsString(ProjectileType));
	return Projectile;
}

void ASLProjectilePoolManager::ReturnProjectile(ASLProjectile* Projectile)
{
	if (!Projectile)
		return;

	EProjectileType Type = Projectile->GetProjectileType();

	Projectile->ResetProjectile();
	ProjectilePools[Type]->Enqueue(Projectile);
	UE_LOG(LogTemp, Log, TEXT("UseProjectile : Return [%s] Projectile"), *UEnum::GetValueAsString(Type));
}

