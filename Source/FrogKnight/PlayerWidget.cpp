// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


bool UPlayerWidget::Initialize()
{
	Super::Initialize();

	MaxHealthAmount = WrapBox_Health->GetAllChildren().Num();
	CurrHealthAmount = MaxHealthAmount;

	MaxWetnessAmount = WrapBox_Wetness->GetAllChildren().Num();
	CurrWetnessAmount = MaxWetnessAmount;

	for (auto& HealthChild : WrapBox_Health->GetAllChildren())
	{
		HealthHearts.Add(TPair<int32, UImage*>(EFilledAmount::Full, Cast<UImage>(HealthChild)));
	}

	for (auto& WetnessChild : WrapBox_Wetness->GetAllChildren())
	{
		WetnessDrops.Add(TPair<int32, UImage*>(EFilledAmount::Full, Cast<UImage>(WetnessChild)));
	}

	return true;
}


void UPlayerWidget::UpdateWetnessBar(float CurrentValue, float MaxValue)
{
	float Percent = CurrentValue / MaxValue;
	ProgressBar_Wetness->SetPercent(Percent);

	FLinearColor NewColour = FLinearColor::LerpUsingHSV(FLinearColor(0.0f, 0.43f, 0.0f), FLinearColor(0.43f, 0.21f, 0.09f), 1 - Percent);
	ProgressBar_Wetness->SetFillColorAndOpacity(NewColour);

	WrapBox_Health->GetChildAt(1);
}

void UPlayerWidget::UpdateHealthBar(float CurrentValue, float MaxValue)
{
	float Percent = CurrentValue / MaxValue;

	int32 Value = Percent / MaxHealthAmount;

	ProgressBar_Health->SetPercent(Percent);
}

void UPlayerWidget::AddHealth()
{
	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TPair<int32, UImage*>& CurrElement = HealthHearts[CurrHealthAmount - 1];
	switch (CurrElement.Key)
	{
	case EFilledAmount::Empty:
		CurrElement.Key = EFilledAmount::HalfFull;
		CurrElement.Value->SetBrushFromTexture(GameInstance->HeartHalf);
		//update with new element value
		break;
	case EFilledAmount::HalfFull:
		CurrElement.Key = EFilledAmount::Full;
		CurrElement.Value->SetBrushFromTexture(GameInstance->HeartFull);
		break;
		//update with new element value
	case EFilledAmount::Full:
		if (CurrHealthAmount - 1 < MaxHealthAmount - 1)
		{
			CurrHealthAmount++;
			HealthHearts[CurrHealthAmount - 1].Key = EFilledAmount::HalfFull;
			HealthHearts[CurrHealthAmount - 1].Value->SetBrushFromTexture(GameInstance->HeartHalf);
			//update it's UI as well
		}
		break;
	}
}

void UPlayerWidget::RemoveHealth()
{
	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TPair<int32, UImage*>& CurrElement = HealthHearts[CurrHealthAmount - 1];
	switch (CurrElement.Key)
	{
	case EFilledAmount::Full:
		CurrElement.Key = EFilledAmount::HalfFull;
		CurrElement.Value->SetBrushFromTexture(GameInstance->HeartHalf);
		//update with new element value
		break;
	case EFilledAmount::HalfFull:
		CurrElement.Key = EFilledAmount::Empty;
		CurrElement.Value->SetBrushFromTexture(GameInstance->HeartEmpty);
		break;
		//update with new element value
	case EFilledAmount::Empty:
		if (CurrHealthAmount - 1 > 0)
		{
			CurrHealthAmount--;
			HealthHearts[CurrHealthAmount - 1].Key = EFilledAmount::HalfFull;
			HealthHearts[CurrHealthAmount - 1].Value->SetBrushFromTexture(GameInstance->HeartHalf);
			//update it's UI as well
		}
		else
		{
			//you the player died
		}
		break;
	}
}

void UPlayerWidget::AddWetness()
{
	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TPair<int32, UImage*>& CurrElement = WetnessDrops[CurrWetnessAmount - 1];
	switch (CurrElement.Key)
	{
	case EFilledAmount::Empty:
		CurrElement.Key = EFilledAmount::HalfFull;
		CurrElement.Value->SetBrushFromTexture(GameInstance->WetHalf);
		//update with new element value
		break;
	case EFilledAmount::HalfFull:
		CurrElement.Key = EFilledAmount::Full;
		CurrElement.Value->SetBrushFromTexture(GameInstance->WetFull); 
		break;
		//update with new element value
	case EFilledAmount::Full:
		if (CurrWetnessAmount - 1< MaxWetnessAmount - 1)
		{
			CurrWetnessAmount++;
			WetnessDrops[CurrWetnessAmount - 1].Key = EFilledAmount::HalfFull;
			WetnessDrops[CurrWetnessAmount - 1].Value->SetBrushFromTexture(GameInstance->WetHalf);
			//update it's UI as well
		}
		break;
	}
}

void UPlayerWidget::RemoveWetness()
{
	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TPair<int32, UImage*>& CurrElement = WetnessDrops[CurrWetnessAmount - 1];
	switch (CurrElement.Key)
	{
	case EFilledAmount::Full:
		CurrElement.Key = EFilledAmount::HalfFull;
		CurrElement.Value->SetBrushFromTexture(GameInstance->WetHalf);
		//update with new element value
		break;
	case EFilledAmount::HalfFull:
		CurrElement.Key = EFilledAmount::Empty;
		CurrElement.Value->SetBrushFromTexture(GameInstance->WetEmpty);
		break;
		//update with new element value
	case EFilledAmount::Empty:
		if (CurrWetnessAmount - 1 > 0)
		{
			CurrWetnessAmount--;
			WetnessDrops[CurrWetnessAmount - 1].Key = EFilledAmount::HalfFull;
			WetnessDrops[CurrWetnessAmount - 1].Value->SetBrushFromTexture(GameInstance->WetHalf);
			//update it's UI as well
		}
		else
		{
			RemoveHealth();
		}
		break;
	}
}