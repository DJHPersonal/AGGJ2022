// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogNightCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFrogNightCharacter::AFrogNightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AFrogNightCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass()));//FindComponentByClass<ACameraActor>();
	if (CameraActor)
	{
		CameraActor->SetActorRotation(CameraInitalRotation);
	}
}

// Called every frame
void AFrogNightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera(DeltaTime);
}

// Called to bind functionality to input
void AFrogNightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AFrogNightCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFrogNightCharacter::Turn);
}

void AFrogNightCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AFrogNightCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}



void AFrogNightCharacter::MoveCamera(float DeltaTime)
{
	if (CameraActor)
	{
		FVector DesiredLocation = GetActorLocation() + CameraOffsetLocation;
		FVector CameraLocation = CameraActor->GetActorLocation();
		FVector DirectionFromCamera = UKismetMathLibrary::Normal(DesiredLocation - CameraActor->GetActorLocation(), 1);
		float Distance = FVector::Dist(DesiredLocation, CameraLocation) / 100;

		CameraActor->SetActorLocation(CameraLocation + DirectionFromCamera * DeltaTime * EasingIn(Distance) * CameraMoveSpeed);
	}
}

float AFrogNightCharacter::EasingIn(float Value)
{
	return FMath::Pow(Value, 2);
}