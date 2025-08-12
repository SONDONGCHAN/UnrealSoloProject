// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SLProjectilePlayerKnife.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

ASLProjectilePlayerKnife::ASLProjectilePlayerKnife()
{
	FlyingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlyingEffect"));
	FlyingEffect->SetupAttachment(CollisionComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlyingEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/SekiroLike/Effect/Kallari/Particles/DaggerThrow/FX/P_Kallari_Dagger_Projectile.P_Kallari_Dagger_Projectile'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_Dagger_Projectile_Rogue.P_Dagger_Projectile_Rogue'
	if (FlyingEffectRef.Object)
	{
		FlyingEffect->SetTemplate(FlyingEffectRef.Object);
		FlyingEffect->bAutoActivate = false;
	}

	ThrowEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrowEffect"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ThrowEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/SekiroLike/Effect/Kallari/Particles/DaggerThrow/FX/P_Kallari_DaggerThrow_Spawn.P_Kallari_DaggerThrow_Spawn'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_DaggerThrow_Spawn_Rogue.P_DaggerThrow_Spawn_Rogue'
	if (ThrowEffectRef.Object)
	{
		ThrowEffect->SetTemplate(ThrowEffectRef.Object);
		ThrowEffect->bAutoActivate = false;
	}

	HitPawnEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitPawnEffect"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitPawnEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/SekiroLike/Effect/Kallari/Particles/DaggerThrow/FX/P_Kallari_DaggerThrow_HitPlayer.P_Kallari_DaggerThrow_HitPlayer'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_DaggerThrow_HitPlayer_Rogue.P_DaggerThrow_HitPlayer_Rogue'
	if (HitPawnEffectRef.Object)
	{
		HitPawnEffect->SetTemplate(HitPawnEffectRef.Object);
		HitPawnEffect->bAutoActivate = false;
	}

	HitEnvironmentEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEnvironmentEffect"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEnvironmentEffecttRef(TEXT("/Script/Engine.ParticleSystem'/Game/SekiroLike/Effect/Kallari/Particles/DaggerThrow/FX/P_Kallari_DaggerThrow_HitWorld.P_Kallari_DaggerThrow_HitWorld'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_DaggerThrow_HitWorld_Rogue.P_DaggerThrow_HitWorld_Rogue'
	if (HitEnvironmentEffecttRef.Object)
	{
		HitEnvironmentEffect->SetTemplate(HitEnvironmentEffecttRef.Object);
		HitEnvironmentEffect->bAutoActivate = false;
	}

	RemoveEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RemoveEffect"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RemoveEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/SekiroLike/Effect/Kallari/Particles/DaggerThrow/FX/P_Kallari_DaggerThrow_NoHit.P_Kallari_DaggerThrow_NoHit'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_DaggerThrow_NoHit_Rogue.P_DaggerThrow_NoHit_Rogue'
	if (RemoveEffectRef.Object)
	{
		RemoveEffect->SetTemplate(RemoveEffectRef.Object);
		RemoveEffect->bAutoActivate = false;
	}

	CollisionComponent->InitSphereRadius(30.0f);
	ProjectileDamage = 100.f;
	ProjectileLifeTime = 1.5f;
}

void ASLProjectilePlayerKnife::BeginPlay()
{
	Super::BeginPlay();
}

void ASLProjectilePlayerKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASLProjectilePlayerKnife::ResetProjectile()
{
	Super::ResetProjectile();
	FlyingEffect->Activate(false);
	FlyingEffect->DeactivateSystem();
}

void ASLProjectilePlayerKnife::ActiveProjectile()
{
	Super::ActiveProjectile();
}

void ASLProjectilePlayerKnife::FireInDirection(const FVector& ShootDirection, float InPower)
{
	Super::FireInDirection(ShootDirection, InPower);
	FlyingEffect->Activate(true);
	FlyingEffect->ActivateSystem();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ThrowEffect->Template, GetActorLocation(), GetActorRotation());
}

void ASLProjectilePlayerKnife::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnProjectileHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	if (OtherActor && OtherActor == GetOwner())
		return;

	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(APawn::StaticClass()))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPawnEffect->Template, GetActorLocation(), GetActorRotation());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEnvironmentEffect->Template, GetActorLocation(), GetActorRotation());
		}
	}
}

void ASLProjectilePlayerKnife::ReturnProjectile()
{
	if (CurrentLifeTime <= 0.f)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RemoveEffect->Template, GetActorLocation(), GetActorRotation());
	}

	Super::ReturnProjectile();
}
