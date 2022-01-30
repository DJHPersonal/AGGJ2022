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
			UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LVL_01_Outside")));
		}
		break;
	}
}

float UPlayerWidget::AddWetness()
{
	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	WarningText->SetText(FText::FromString(""));

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
		else
		{
			RemoveHealth();
			WarningText->SetText(FText::FromString("Too Wet"));
		}
		break;
	}
	return CurrWetnessAmount / (float)MaxWetnessAmount;
}

float UPlayerWidget::RemoveWetness()
{
	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	WarningText->SetText(FText::FromString(""));

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
			WarningText->SetText(FText::FromString("Drying Out"));
		}
		break;
	}
	return CurrWetnessAmount / (float)MaxWetnessAmount;
}