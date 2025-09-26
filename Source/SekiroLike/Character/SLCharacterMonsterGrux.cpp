// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterMonsterGrux.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/SLWidgetComponent.h"
#include "AI/SLAI.h"
#include "AI/SLAIController.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASLCharacterMonsterGrux::ASLCharacterMonsterGrux()
{
	GetCapsuleComponent()->InitCapsuleSize(60.f, 100.0f);

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterSkeletalMesh(TEXT("/Script/Engine.Skeleton'/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux_Skeleton.Grux_Skeleton'"));
	if (CharacterSkeletalMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterSkeletalMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/SekiroLike/Animation/Monster_Grux/ABP_SLMonsterGrux.ABP_SLMonsterGrux_C"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	// Anim Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/Monster_Grux/AM_GruxDeath.AM_GruxDeath'"));
	if (DeadMontageRef.Object)
	{
		DeathMontage = DeadMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/Monster_Grux/AM_NormalAttack.AM_NormalAttack'"));
	if (NormalAttackMontageRef.Object)
	{
		NormalAttackMontage = NormalAttackMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RushAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/Monster_Grux/AM_RushAttack.AM_RushAttack'"));
	if (RushAttackMontageRef.Object)
	{
		RushAttackMontage = RushAttackMontageRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/Monster_Grux/AM_GruxHitReact.AM_GruxHitReact'"));
	if (HitReactMontageRef.Object)
	{
		HitReactMontage = HitReactMontageRef.Object;
	}

	/*Effect*/
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGrux/FX/Particles/Abilities/Primary/FX/P_Grux_Melee_SucessfulImpact.P_Grux_Melee_SucessfulImpact'"));
	if (HitEffectRef.Object)
	{
		HitEffect->SetTemplate(HitEffectRef.Object);
		HitEffect->bAutoActivate = false;
	}

	// Widget Component
	HpBar = CreateDefaultSubobject<USLWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/SekiroLike/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASLCharacterMonsterGrux::SetDeath()
{
	Super::SetDeath();

}

float ASLCharacterMonsterGrux::GetAIPatrolRadius()
{
	float Result = Super::GetAIPatrolRadius();
	
	return Result;
}

float ASLCharacterMonsterGrux::GetAIDetectRange()
{
	float Result = Super::GetAIDetectRange();

	return 1000.f;
}

float ASLCharacterMonsterGrux::GetAITurnSpeed()
{
	float Result = Super::GetAITurnSpeed();

	return 4.f;
}

void ASLCharacterMonsterGrux::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	Super::SetAIAttackDelegate(InOnAttackFinished);
}

void ASLCharacterMonsterGrux::AttackByAI(EAttackType InAttackType)
{
	Super::AttackByAI(InAttackType);

	switch (InAttackType)
	{
	case EAttackType::AttackNormal:
		NormalAttack();
		break;

	case EAttackType::Skill_A:
		RushAttack();
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown AttackType in AttackByAI"));
		break;
	}

}

float ASLCharacterMonsterGrux::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Reuslt = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(GetIsDead())
		return Reuslt;

	if (!bIsHitReacting)
	{
		ASLAIController* SLAIController = Cast<ASLAIController>(GetController());
		if (SLAIController)
		{
			SLAIController->StopAI();
		}
	}
	
	HitReact();
	
	return Reuslt;
}

FHitResult ASLCharacterMonsterGrux::AttackHitCheckSphereSweep(FString AttackName)
{
	FHitResult HitResult = Super::AttackHitCheckSphereSweep(AttackName);
	
	if (HitResult.bBlockingHit)
	{
		FVector HitLocation = HitResult.ImpactPoint;
		FVector HitNormal = HitResult.ImpactNormal;
		FRotator HitRotation = HitNormal.Rotation();
	
		//if (HitResult.bStartPenetrating)
		//{
		//	FVector TargetLocation = HitResult.GetActor()->GetActorLocation();
		//	float Radius = Cast<ACharacter>(HitResult.GetActor())->GetCapsuleComponent()->GetScaledCapsuleRadius();
		//	TargetLocation = { TargetLocation.X, TargetLocation.Y, HitLocation.Z };
		//	HitLocation = TargetLocation - (HitNormal * Radius);
		//}
	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect->Template, HitLocation, HitRotation);
	}
	
	return HitResult;
}

void ASLCharacterMonsterGrux::SetupCharacterWidget(USLUserWidget* InUserWidget)
{
	Super::SetupCharacterWidget(InUserWidget);

}

void ASLCharacterMonsterGrux::NormalAttack()
{
	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float SpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(NormalAttackMontage, SpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterMonsterGrux::NormalAttacknEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, NormalAttackMontage);
}

void ASLCharacterMonsterGrux::NormalAttacknEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(NormalAttackMontage) || GetIsDead())
		return;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void ASLCharacterMonsterGrux::RushAttack()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	const float SpeedRate = 0.9f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RushAttackMontage, SpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterMonsterGrux::RushAttacknEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RushAttackMontage);
}

void ASLCharacterMonsterGrux::RushAttacknEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void ASLCharacterMonsterGrux::HitReact()
{
	bIsHitReacting = true;
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(HitReactMontage))
	{
		AnimInstance->Montage_Stop(0.f, HitReactMontage);
	}
	
	const float SpeedRate = 1.f;
	AnimInstance->Montage_Play(HitReactMontage, SpeedRate);
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterMonsterGrux::HitReactEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, HitReactMontage);
}

void ASLCharacterMonsterGrux::HitReactEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(HitReactMontage) || GetIsDead())
		return;

	bIsHitReacting = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
	ASLAIController* SLAIController = Cast<ASLAIController>(GetController());
	if (SLAIController)
	{
		SLAIController->RunAI();
	}
}
