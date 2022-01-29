// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class FROGKNIGHT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMainGameInstance(const FObjectInitializer& ObjectInitilize);

	TSubclassOf<UUserWidget> MainMenuClass;
	UUserWidget* MainMenuWidget;

	UFUNCTION(BlueprintCallable)
	void SetupMainMenu();

	TSubclassOf<UUserWidget> PlayerWidgetClass;
	UUserWidget* PlayerWidget;

	UFUNCTION(BlueprintCallable)
	void SetupPlayerWidget();

	//the various UI textures
	UTexture2D* WetFull;
	UTexture2D* WetHalf;
	UTexture2D* WetEmpty;

	UTexture2D* HeartFull;
	UTexture2D* HeartHalf;
	UTexture2D* HeartEmpty;
};
