// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/SLCharacterHUDInterface.h"
#include "Interface/SLPlayerSkillCheckInterface.h"
#include "GameData/SLGeneralData.h"
#include "SLCharacterPlayer.generated.h"

class ASLFreeCamera;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UParticleSystemComponent;
class USLTargetSystemComponent;
/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLCharacterPlayer : public ASLCharacterBase, public ISLCharacterHUDInterface, public ISLPlayerSkillCheckInterface
{
	GENERATED_BODY()
	
public:
	ASLCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetDeath() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class USLCharacterControlData* CharacterControlData) override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

// FreeCam Section
protected:
	UPROPERTY()
	ASLFreeCamera* FreeCameraPawn;

	bool          bIsUsingFreeCamera = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleFreeCamAction;

public:
	void ToggleFreeCamera(const FInputActionValue& Value);

// Input Section
protected:
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> StealthWalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RushAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TargetingThrowingKnifeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShadowStrikeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EvadeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LockOnAction;


	virtual void Jump();
		
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void Attack();
	virtual FHitResult ComboAttackHitCheck() override;

// HUD Section
protected:
	virtual void SetupHUDWidget(class USLHUDWidget* InHUDWidget) override;


// Movement Section
private:
	void StartSprint();
	void EndSprint();

	UPROPERTY(EditAnywhere, Category = Movement)
	float StealthWalkSpeed;

	UPROPERTY(EditAnywhere, Category = Movement)
	float NormalSpeed;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintSpeed;

	float TargetSpeed = 0;
	bool bSmoothing = false;

	//Dodge Section
public:
	virtual void Dodge(const FVector DodgeDir, const float StartSpeed, const float DodgeTime, const ECurveType CurveType) override;
protected:
	virtual void DodgeTick(float DeltaTime) override;


	// Stealth Section
protected:
	void ToggleStealthMode();
	void OnStealth();
	void OffStealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stealth)
	TArray<UMaterialInterface*> TransparencyMaterials;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stealth)
	TArray<UMaterialInterface*> BasicMaterials;
	
	bool bIsStealth = false;

public:
	virtual bool CanDetect() override;

	// Skill Section
private:
	bool CheckMP(float Mp);
	void SkillTick(float DeltaTime);
	bool bIsActing = false;

	/*Combo Attack*/
protected:
	virtual void ComboActionBegin() override;
	virtual void NotifyComboActionEnd()override;

	/*Rush Attack*/
protected:
	void RushAttack();
	void RushAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void RushAttackHitCheck() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RushAttackMontage;

	/*Throwing Knife*/
protected:
	void TargetingThrowingKnife();
	void CancelThrowingKnife();
	void ThrowKnife();
	void ThrowingKnifeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ThrowingKnifeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> CancelThrowingKnifeMontage;

private:
	bool bIsTargeting = false;

	/*Shadow Strike*/
protected:
	void ShadowStrike();
	void ShadowStrikeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void ShadowStrikeHitCheck() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ShadowStrikeMontage;

	/*Evade*/
protected:
	void Evade();
	void EvadeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> EvadeMontage;

private:
	bool bIsCanceling = false;

	/*Effect*/
protected:
	/*Hit*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> HitEffect;

	/*Stealth*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> StealthEnterEffect;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> StealthExitEffect;

	/*Combo Attack*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> ComboEffect_L;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> ComboEffect_R;
	
	/*Rush Attack*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> BurstEffect;
	
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> RushWeaponEffect_L;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> RushWeaponEffect_R;
	
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> RushWindEffect;


	/*Throwing Knife*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> TargetingEffect;

	/*Evade*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> DodgeEffect1;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> DodgeEffect2;

	/*Shadow Strike*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> ShadowStrikeWeaponEffect_L;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> ShadowStrikeWeaponEffect_R;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> ShadowStrikeBodyEffect;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> ShadowStrikeHitEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> BloodEffect_LH;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> BloodEffect_RH;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> BloodEffect_LF;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> BloodEffect_RF;

	/*MpEffect*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> MpEffect_LH;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> MpEffect_RH;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> MpEffect_LF;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> MpEffect_RF;


	/*TargetSystem*/
protected:
	void ToggleLockOn();

private:
	void LockOn();
	void LockOff();
	void LockOnTick(float DeltaTime);
	void UpdateTarget(APawn* InPawn);
	bool bIsLockOn = false;

protected:
	UPROPERTY(EditAnywhere, Category = TargetSystem)
	TObjectPtr<USLTargetSystemComponent> TargetSystem;


	//Hit React Section
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void HitReact();
	void HitReactEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
protected:
	bool bIsHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HitReactMontage;

	//Motion Warping
protected:
	void SetMotionWarpTarget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MotionWarping)
	TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;

};
