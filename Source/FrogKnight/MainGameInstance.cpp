 // Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.h"

UMainGameInstance::UMainGameInstance(const FObjectInitializer& ObjectInitilize)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuObject(TEXT("/Game/UIWidgets/MainMenuWidget"));
	MainMenuClass = MainMenuObject.Class; //get the file location of the widget blueprint class and store it in this variable

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerWidgetObject(TEXT("/Game/UIWidgets/PlayerWidget"));
	PlayerWidgetClass = PlayerWidgetObject.Class; //get the file location of the widget blueprint class and store it in this variable
}

void UMainGameInstance::SetupMainMenu()
{
	if (MainMenuClass)
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuClass); //spawn in a new widget

	if (MainMenuWidget) //as exists now, add it to the viewport
		MainMenuWidget->AddToViewport();

	FInputModeUIOnly InputMode; //gets the mouse to appear on screen and unlock cursor from menu widget
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());

	APlayerController* PlayerController;
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(InputMode); //tell current controller of game to use these input settings
		PlayerController->bShowMouseCursor = true; //don't hide cursor on mouse down
	}
}

void UMainGameInstance::SetupPlayerWidget()
{
	if (PlayerWidgetClass)
		PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass); //spawn in a new widget

	if (PlayerWidget) //as exists now, add it to the viewport
	{
		PlayerWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("Player Widget Added"))
	}
	FInputModeGameOnly InputMode; //gets the mouse to appear on screen and unlock cursor from menu widget

	APlayerController* PlayerController;
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
		PlayerController->SetInputMode(InputMode); //tell current controller of game to use these input settings
}