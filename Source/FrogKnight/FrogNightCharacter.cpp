// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogNightCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerWidget.h"

// Sets default values
AFrogNightCharacter::AFrogNightCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentMoveSpeed = MaxMoveSpeed;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;

	MaxWetness = 100;
	Wetness = MaxWetness;
	WetnessReduction = 1;
	bInWater = false;

	MovementDirection = FVector(1, 0, 0);
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

	OnActorBeginOverlap.AddDynamic(this, &AFrogNightCharacter::OnOverlap);
	OnActorEndOverlap.AddDynamic(this, &AFrogNightCharacter::EndOverlap);

	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this));
}

// Called every frame
void AFrogNightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera(DeltaTime);
	SetOrientation(DeltaTime);

	if (!bInWater && Wetness > 0)
		UpdateWetness(-DeltaTime * WetnessReduction);
	else if(Wetness < MaxWetness)
		UpdateWetness(DeltaTime * WetnessReduction * 4);
}

// Called to bind functionality to input
void AFrogNightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AFrogNightCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &AFrogNightCharacter::Strafe);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFrogNightCharacter::Jump);
}

void AFrogNightCharacter::MoveForward(float Value) //setup movement so it works by orienting the character in the  direction of movement
{
	AddMovementInput(GetActorForwardVector(), Value);
	MovementDirection.Y = Value;
}

void AFrogNightCharacter::Strafe(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
	MovementDirection.X = Value;
}

//void AFrogNightCharacter::SetOrientation(float DeltaTime)
//{
//	//float TargetAngle = FMath::RadiansToDegrees(FMath::Atan2(MovementDirection.X, MovementDirection.Y));
//	//
//	////
//	//SetActorRotation(FRotator(0.0f, TargetAngle, 0.0f));
//}

void AFrogNightCharacter::Jump()
{
	Super::Jump();
	if (bPressedJump && CanJump())
	{
		GetCharacterMovement()->Velocity.X += FMath::Max(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->JumpZVelocity / 2) * GetActorForwardVector().X;
		GetCharacterMovement()->Velocity.Y += FMath::Max(GetCharacterMovement()->Velocity.Y, GetCharacterMovement()->JumpZVelocity / 2) * GetActorForwardVector().Y;
		UE_LOG(LogTemp, Warning, TEXT("Forward Velocity: %f, %f"), GetActorForwardVector().X, GetActorForwardVector().Y)
	}
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
	return FMath::Pow(Value, 1.75);
}



void AFrogNightCharacter::UpdateWetness(float UpdateValue)
{
	Wetness += UpdateValue;
	if (Wetness > 0)
	{
		if (GameInstance && GameInstance->PlayerWidget)
		{
			GameInstance->PlayerWidget->UpdateWetnessBar(Wetness, MaxWetness);
		}
	}
	else //lose health
	{
		UpdateHealth(-GetWorld()->GetDeltaSeconds() * 20);
	}
}
void AFrogNightCharacter::UpdateHealth(float UpdateValue)
{
	CurrentHealth += UpdateValue;
	if (GameInstance && GameInstance->PlayerWidget)
	{
		GameInstance->PlayerWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void AFrogNightCharacter::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor->GetName() == FString(TEXT("TestWater")))
		bInWater = true;
}

void AFrogNightCharacter::EndOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (OtherActor->GetName() == FString(TEXT("TestWater")))
		bInWater = false;
}
