// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FROGKNIGHT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* Button_Start;
	UPROPERTY(meta = (BindWidget))
	UButton* Button_Quit;

	UFUNCTION()
	void OnStartButtonPressed();
	UFUNCTION()
	void OnQuitButtonPressed();
};
