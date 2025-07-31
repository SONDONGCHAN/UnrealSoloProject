// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SLProjectile.h"
#include "Projectile/SLProjectilePoolManager.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DamageEvents.h"
#include "Physics/SLCollision.h"



// Sets default values
ASLProjectile::ASLProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(25.0f);
	CollisionComponent->SetCollisionProfileName(CPROFILE_SLSPHERE);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASLProjectile::OnProjectileHit);
	SetRootComponent(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 1000.0f; // 초기 속도
	ProjectileMovementComponent->MaxSpeed = 1000.0f;     // 최대 속도
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // 방향 자동 보정
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;   // 중력 영향 X 

	ProjectileDamage = 100.f;
	ProjectileLifeTime = 1.f;
}

// Called when the game starts or when spawned
void ASLProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASLProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLifeTime -= DeltaTime;
	if(CurrentLifeTime <= 0.f)
	{
		ReturnProjectile();
	}
	else
	{
//#if ENABLE_DRAW_DEBUG
//		DrawDebugAccum += DeltaTime;
//		if (DrawDebugAccum >= DrawDebugInterval)
//		{
//			DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionComponent->GetScaledSphereRadius(), 16, FColor::Green, false, CurrentLifeTime);
//			DrawDebugAccum = 0.0f;
//		}
//#endif
	}


}

void ASLProjectile::ResetProjectile()
{
	ProjectileMovementComponent->StopMovementImmediately();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	ResetLifeTime();
	//if (ProjectileMovementComponent && !(ProjectileMovementComponent->UpdatedComponent))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("UpdatedComponent is Nullptr"));
	//}
}

void ASLProjectile::ActiveProjectile()
{
	if (ProjectileMovementComponent && !(ProjectileMovementComponent->UpdatedComponent))
	{
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	}
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ASLProjectile::FireInDirection(const FVector& ShootDirection, float InPower)
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = InPower;
		ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
		//ProjectileMovementComponent->UpdateComponentVelocity();
	}
}

void ASLProjectile::InitializeProjectile(EProjectileType InProjectileType)
{
	MyProjectileType = InProjectileType;

}

void ASLProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 이벤트 발생 시 데미지 적용

	if (OtherActor && OtherActor == GetOwner())
		return;

	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(APawn::StaticClass()))
		{
			float MyDamage = ProjectileDamage;
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(MyDamage, DamageEvent, Cast<APawn>(GetOwner())->GetController(), this);
		}
		else
		{

		}

		ReturnProjectile();
	}
}

void ASLProjectile::ResetLifeTime()
{
	CurrentLifeTime = ProjectileLifeTime;
}

void ASLProjectile::ReturnProjectile()
{
	ASLProjectilePoolManager* ProjectilePoolManager = ASLProjectilePoolManager::GetInstance(GetWorld());
	if (ProjectilePoolManager)
	{
		ProjectilePoolManager->ReturnProjectile(this);
	}
}

