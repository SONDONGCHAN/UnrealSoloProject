// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/SLCharacterHUDInterface.h"
#include "GameData/SLGeneralData.h"
#include "SLCharacterPlayer.generated.h"

class ASLFreeCamera;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLCharacterPlayer : public ASLCharacterBase, public ISLCharacterHUDInterface
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


	virtual void Jump();
		
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void Attack();

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

public:
	void Dodge(const FVector DodgeDir, const float DodgeDis, const float DodgeTime, const ECurveType CurveType);

private:
	void DodgeTick(float DeltaTime);
	FVector DodgeDirection = FVector::ZeroVector;
	float DodgeTimeTotal = 0;
	float DodgeTimeLeft = 0;
	bool bIsDodge= false;
	ECurveType CurrentCurveType;

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

// Skill Section
private:
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
};
