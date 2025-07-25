// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SLAnimationAttackInterface.h"
#include "Interface/SLCharacterWidgetInterface.h"
#include "Interface/SLCharacterItemInterface.h"
#include "GameData/SLCharacterStat.h"
#include "GameData/SLGeneralData.h"
#include "SLCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSLCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class USLItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class SEKIROLIKE_API ASLCharacterBase : public ACharacter, public ISLAnimationAttackInterface, public ISLCharacterWidgetInterface, public ISLCharacterItemInterface
{
	GENERATED_BODY()

public:
	ASLCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class USLCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class USLCharacterControlData*> CharacterControlManager;

//Combo Action Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Death Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeathMontage;

	virtual void SetDeath();
	void PlayDeathAnimation();

	float DeathEventDelayTime = 5.f;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLCharacterStatComponent> Stat;
		 
// UI Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(USLUserWidget* InUserWidget) override;

// Item Section
protected:

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class USLItemData* InItemData) override;

	virtual void DrinkPotion(class USLItemData* InItemData);
	virtual void BoostPower(class USLItemData* InItemData);
	virtual void EnhanceStat(class USLItemData* InItemData);


// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat);


//CharacterType Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterType)
	ECharacterType CharacterType;
};
