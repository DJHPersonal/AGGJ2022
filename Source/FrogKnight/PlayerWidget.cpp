// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"

void UPlayerWidget::UpdateWetnessBar(float CurrentValue, float MaxValue)
{
	float Percent = CurrentValue / MaxValue;
	ProgressBar_Wetness->SetPercent(Percent);

	FLinearColor NewColour = FLinearColor::LerpUsingHSV(FLinearColor(0.0f, 0.43f, 0.0f), FLinearColor(0.43f, 0.21f, 0.09f), 1 - Percent);
	ProgressBar_Wetness->SetFillColorAndOpacity(NewColour);
}

void UPlayerWidget::UpdateHealthBar(float CurrentValue, float MaxValue)
{
	float Percent = CurrentValue / MaxValue;
	ProgressBar_Health->SetPercent(Percent);
}
