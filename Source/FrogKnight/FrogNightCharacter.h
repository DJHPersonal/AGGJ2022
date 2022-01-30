// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "MainGameInstance.h"

#include "FrogNightCharacter.generated.h"

UCLASS()
class FROGKNIGHT_API AFrogNightCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFrogNightCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadOnly)
	FVector MovementDirection;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetOrientation(float DeltaTime, float CameraRotation);
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerColour(float Value);
private:
	UMainGameInstance* GameInstance;

	//stuff for the player movement
	void MoveForward(float Value);
	void Strafe(float Value);

	void Jump() override;

	int32 ForwardDirection;
	float TurnSpeed;
	float MaxMoveSpeed;
	float CurrentMoveSpeed;



	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	ACameraActor* CameraActor;
	//UPROPERTY(EditAnywhere, Category = "Camera Stats")
	FVector CameraOffsetLocation;
	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	FRotator CameraInitalRotation;	
	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	float CameraMoveSpeed;

	float CameraAngleHorizontal;
	float CameraAngleVertical;
	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	float CameraHorizontalSenstivity;
	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	float CameraVerticalSenstivity;
	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	float CameraDistance;
	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	float CameraLowerBound;
	UPROPERTY(EditAnywhere, Category = "Camera Stats")
	float CameraUpperBound;


	float EasingIn(float Value);
	void MoveCamera(float DeltaTime);
	void CameraZoomIn();
	void CameraZoomOut();

	bool bInWater;
	void UpdateWetness(float UpdateValue);
	UFUNCTION()
	void ReduceWetness();
	UFUNCTION()
	void AddWetness();
	void UpdateHealth(float UpdateValue);

	//collision detection
	UFUNCTION()
	void BeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor); 
	UFUNCTION()
	void EndOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	//timers for the reduction of the wetness
	FTimerHandle WetnessTimer;
};
