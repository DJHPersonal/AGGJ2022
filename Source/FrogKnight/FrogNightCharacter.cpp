// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogNightCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerWidget.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"


// Sets default values
AFrogNightCharacter::AFrogNightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentMoveSpeed = MaxMoveSpeed;

	bInWater = false;

	MovementDirection = FVector(1, 0, 0);

	CameraAngleHorizontal = 0.0f;
	CameraAngleVertical = 0.0f;
	CameraHorizontalSenstivity = 1.0f;
	CameraVerticalSenstivity = 1.0f;
	CameraDistance = 1000.0f;
	CameraLowerBound = 10.0f;
	CameraUpperBound = 40.0f;
}

// Called when the game starts or when spawned
void AFrogNightCharacter::BeginPlay()
{
	Super::BeginPlay();
	//CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass()));//FindComponentByClass<ACameraActor>();
	if (CameraActor)
	{
		CameraActor->SetActorRotation(CameraInitalRotation);
	}

	OnActorBeginOverlap.AddDynamic(this, &AFrogNightCharacter::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AFrogNightCharacter::EndOverlap);

	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this));

	//setup the reduction timer
	GetWorldTimerManager().SetTimer(WetnessTimer, this, &AFrogNightCharacter::ReduceWetness, 5, true, 5);

}

// Called every frame
void AFrogNightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera(DeltaTime);
	SetOrientation(DeltaTime, CameraActor->GetActorRotation().Euler().Z);
}

// Called to bind functionality to input
void AFrogNightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AFrogNightCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &AFrogNightCharacter::Strafe);

	PlayerInputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &AFrogNightCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &AFrogNightCharacter::CameraZoomOut);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFrogNightCharacter::Jump);
}

void AFrogNightCharacter::MoveForward(float Value) //setup movement so it works by orienting the character in the  direction of movement
{
	FVector Forward = CameraActor->GetActorForwardVector();
	Forward.Z = 0;

	AddMovementInput(Forward, Value);
	MovementDirection.Y = Value;
}

void AFrogNightCharacter::Strafe(float Value)
{
	FVector Right = CameraActor->GetActorRightVector();
	AddMovementInput(Right, Value);
	MovementDirection.X = Value;
}

void AFrogNightCharacter::Jump()
{
	Super::Jump();
	if (bPressedJump && CanJump())
	{
		GetCharacterMovement()->Velocity.X += FMath::Max(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->JumpZVelocity / 2) * CameraActor->GetActorForwardVector().X;
		GetCharacterMovement()->Velocity.Y += FMath::Max(GetCharacterMovement()->Velocity.Y, GetCharacterMovement()->JumpZVelocity / 2) * CameraActor->GetActorForwardVector().Y;
	}
}

void AFrogNightCharacter::MoveCamera(float DeltaTime)
{
	if (CameraActor)
	{
		//work out camera location (orbit movemnt)
		float mouseDeltaX, mouseDeltaY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputMouseDelta(mouseDeltaX, mouseDeltaY);
		UKismetMathLibrary::FMod(CameraAngleHorizontal + mouseDeltaX * CameraHorizontalSenstivity, 360.0f, CameraAngleHorizontal);
		UKismetMathLibrary::FMod(CameraAngleVertical + mouseDeltaY * CameraVerticalSenstivity, 360.0f, CameraAngleVertical);
		CameraAngleVertical = FMath::Clamp(CameraAngleVertical, CameraLowerBound, CameraUpperBound);
		float cameraAngleVerticalRadians = FMath::DegreesToRadians(90.0f - CameraAngleVertical);
		float cameraAngleHorizontalRadians = FMath::DegreesToRadians(CameraAngleHorizontal);
		float x = CameraDistance * FMath::Cos(cameraAngleHorizontalRadians) * FMath::Sin(cameraAngleVerticalRadians);
		float y = CameraDistance * FMath::Sin(cameraAngleHorizontalRadians) * FMath::Sin(cameraAngleVerticalRadians);
		float z = CameraDistance * FMath::Cos(cameraAngleVerticalRadians);
		CameraOffsetLocation = FVector(x,y,z);
		
		//move camera
		FVector DesiredLocation = GetActorLocation() + CameraOffsetLocation;
		FVector CameraLocation = CameraActor->GetActorLocation();
		FVector DirectionFromCamera = UKismetMathLibrary::Normal(DesiredLocation - CameraActor->GetActorLocation(), 1);
		float Distance = FVector::Dist(DesiredLocation, CameraLocation) / 100;
		CameraActor->SetActorLocation(CameraLocation + DirectionFromCamera * DeltaTime * EasingIn(Distance) * CameraMoveSpeed);
		CameraActor->SetActorRotation((GetActorLocation() - CameraActor->GetActorLocation()).Rotation());
	}
}

float AFrogNightCharacter::EasingIn(float Value)
{
	return FMath::Pow(Value, 1.75);
}

void AFrogNightCharacter::CameraZoomOut()
{
	CameraDistance -= 25;
	CameraDistance = FMath::Clamp(CameraDistance, 200.0f, 1500.0f);
}
void AFrogNightCharacter::CameraZoomIn()
{
	CameraDistance += 25;
	CameraDistance = FMath::Clamp(CameraDistance, 200.0f, 1500.0f);
}

void AFrogNightCharacter::ReduceWetness()
{
	float Value = Cast<UPlayerWidget>(GameInstance->PlayerWidget)->RemoveWetness();
	PlayerColour(1 - Value);
}
void AFrogNightCharacter::AddWetness()
{
	float Value = Cast<UPlayerWidget>(GameInstance->PlayerWidget)->AddWetness();
	PlayerColour(1 - Value);
}

void AFrogNightCharacter::BeginOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor->GetName() == FString(TEXT("TestWater")))
	{
		bInWater = true;
		GetWorldTimerManager().ClearTimer(WetnessTimer);
		GetWorldTimerManager().SetTimer(WetnessTimer, this, &AFrogNightCharacter::AddWetness, 0.5f, true, 0.5f);

	}
}

void AFrogNightCharacter::EndOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor->GetName() == FString(TEXT("TestWater")))
	{
		bInWater = false;
		GetWorldTimerManager().ClearTimer(WetnessTimer);
		GetWorldTimerManager().SetTimer(WetnessTimer, this, &AFrogNightCharacter::ReduceWetness, 5.0f, true, 5.0f);
	}
}
