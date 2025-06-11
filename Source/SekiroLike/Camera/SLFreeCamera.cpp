// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/SLFreeCamera.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Character/SLCharacterPlayer.h"

// Sets default values
ASLFreeCamera::ASLFreeCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	BasicCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BasicCamera"));
	BasicCamera->SetupAttachment(RootScene);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
	MovementComponent->UpdatedComponent = RootScene;

	// Pawn 회전 설정 변경 (Pitch/Yaw/Roll 모두 허용)
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// 카메라 회전 동기화
	BasicCamera->bUsePawnControlRotation = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_FreeMove.IA_FreeMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		FreeMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_FreeLook.IA_FreeLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		FreeLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/SekiroLike/Input/IMC_Free.IMC_Free'"));
	if (InputMappingContextRef.Object)
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleFreeCam(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_ToggleFreeCam.IA_ToggleFreeCam'"));
	if (nullptr != InputActionToggleFreeCam.Object)
	{
		ToggleFreeCamAction = InputActionToggleFreeCam.Object;
	}
}

// Called when the game starts or when spawned
void ASLFreeCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ASLFreeCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(FreeMoveAction, ETriggerEvent::Triggered, this, &ASLFreeCamera::FreeMove);
		Input->BindAction(FreeLookAction, ETriggerEvent::Triggered, this, &ASLFreeCamera::FreeLook);
		Input->BindAction(ToggleFreeCamAction, ETriggerEvent::Triggered, this, &ASLFreeCamera::HandleToggleFreeCamera);
	}
}

void ASLFreeCamera::FreeMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.IsNearlyZero()) return;
	
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();

	AddMovementInput(Forward, MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds());
	AddMovementInput(Right, MovementVector.Y * MoveSpeed * GetWorld()->GetDeltaSeconds());
}

void ASLFreeCamera::FreeLook(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);

	
	// 카메라 회전 동기화
	BasicCamera->bUsePawnControlRotation = true;
}

void ASLFreeCamera::HandleToggleFreeCamera(const FInputActionValue& Value)
{
	ASLCharacterPlayer* OwnerCharacter = Cast<ASLCharacterPlayer>(OwnerRef);

	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerRef is nullptr"));
		return;
	}

	APlayerController* PC = CastChecked<APlayerController>(GetController());
	PC->Possess(OwnerCharacter);
	OwnerCharacter->ToggleFreeCamera(Value);
}
