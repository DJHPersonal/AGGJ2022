// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class FROGKNIGHT_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateWetnessBar(float CurrentValue, float MaxValue);
	void UpdateHealthBar(float CurrentValue, float MaxValue);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Wetness;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Health;
};
