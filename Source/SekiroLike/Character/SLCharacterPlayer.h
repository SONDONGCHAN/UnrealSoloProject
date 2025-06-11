// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "InputActionValue.h"
#include "SLCharacterPlayer.generated.h"

class ASLFreeCamera;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLCharacterPlayer : public ASLCharacterBase
{
	GENERATED_BODY()
	
public:
	ASLCharacterPlayer();

protected:
	virtual void BeginPlay() override;

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

	virtual void Jump();
		
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

	void Attack();

private:
	// Movement Section
	void StartSprint();
	void EndSprint();

	UPROPERTY(EditAnywhere, Category = Movement)
	float StealthWalkSpeed;

	UPROPERTY(EditAnywhere, Category = Movement)
	float NormalSpeed;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintSpeed;

protected:
	// Stealth Section
	void ToggleStealthMode();
	void OnStealth();
	void OffStealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stealth)
	TArray<UMaterialInterface*> TransparencyMaterials;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stealth)
	TArray<UMaterialInterface*> BasicMaterials;
	
	bool bIsStealth = false;


};
