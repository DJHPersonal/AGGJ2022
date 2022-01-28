// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"

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

private:
	void MoveForward(float Value);
	void Turn(float Value);

	ACameraActor* CameraActor;
	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector CameraOffsetLocation;
	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator CameraInitalRotation;	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMoveSpeed;

	void MoveCamera(float DeltaTime);
};
