// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	//setup the button inputs
	Button_Start->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonPressed);
	Button_Quit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonPressed);

	return true;
}

void UMainMenuWidget::OnStartButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LVL_01_Outside")));
}

void UMainMenuWidget::OnQuitButtonPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
