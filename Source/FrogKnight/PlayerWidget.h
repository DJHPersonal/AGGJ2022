// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "MainGameInstance.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UENUM()
enum EFilledAmount
{
	Empty,
	HalfFull,
	Full,
};

UCLASS()
class FROGKNIGHT_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateWetnessBar(float CurrentValue, float MaxValue);
	void UpdateHealthBar(float CurrentValue, float MaxValue);

	void AddHealth();
	void RemoveHealth();

	float AddWetness();
	float RemoveWetness();

	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_Health;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_Wetness;

	TArray<TPair<int32, UImage*>> HealthHearts;
	TArray<TPair<int32, UImage*>> WetnessDrops;

	int32 MaxHealthAmount;
	int32 CurrHealthAmount;

	int32 MaxWetnessAmount;
	int32 CurrWetnessAmount;

	UMainGameInstance* GameInstance;
};
