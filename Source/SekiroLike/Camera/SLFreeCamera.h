// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "SLFreeCamera.generated.h"

class USceneComponent;
class UCameraComponent;
class UInputAction;
class UFloatingPawnMovement;
class UInputMappingContext;

UCLASS()
class SEKIROLIKE_API ASLFreeCamera : public APawn
{
	GENERATED_BODY()

public:
	ASLFreeCamera();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	TObjectPtr<UInputMappingContext> GetInputMappingContext() { return InputMappingContext; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> BasicCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FreeMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FreeLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FreeCamera")
	float MoveSpeed = 600.f;

	void FreeMove(const FInputActionValue& Value);
	void FreeLook(const FInputActionValue& Value);
	void HandleToggleFreeCamera(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleFreeCamAction;

private:
	UPROPERTY()
	TObjectPtr<AActor> OwnerRef;

public:
	void SetOwnerRef(AActor* pOwner) { OwnerRef = pOwner; }
};
