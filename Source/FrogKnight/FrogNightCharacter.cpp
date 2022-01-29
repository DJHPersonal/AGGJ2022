// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogNightCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerWidget.h"
#include "TimerManager.h"

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

void AFrogNightCharacter::Jump()
{
	Super::Jump();
	if (bPressedJump && CanJump())
	{
		GetCharacterMovement()->Velocity.X += FMath::Max(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->JumpZVelocity / 2) * GetActorForwardVector().X;
		GetCharacterMovement()->Velocity.Y += FMath::Max(GetCharacterMovement()->Velocity.Y, GetCharacterMovement()->JumpZVelocity / 2) * GetActorForwardVector().Y;
		UE_LOG(LogTemp, Warning, TEXT("Forward Velocity: %f, %f"), GetActorForwardVector().X, GetActorForwardVector().Y)
		/*if (GameInstance && GameInstance->PlayerWidget)
		{
			Cast<UPlayerWidget>(GameInstance->PlayerWidget)->RemoveWetness();
		}*/
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

void AFrogNightCharacter::ReduceWetness()
{
	Cast<UPlayerWidget>(GameInstance->PlayerWidget)->RemoveWetness();
}
void AFrogNightCharacter::AddWetness()
{
	Cast<UPlayerWidget>(GameInstance->PlayerWidget)->AddWetness();
}

void AFrogNightCharacter::UpdateWetness(float UpdateValue)
{
	//now it should actually be either add one or remove one


	Wetness += UpdateValue;
	if (Wetness > 0)
	{
		if (GameInstance && GameInstance->PlayerWidget)
		{
			Cast<UPlayerWidget>(GameInstance->PlayerWidget)->UpdateWetnessBar(Wetness, MaxWetness);
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
		Cast<UPlayerWidget>(GameInstance->PlayerWidget)->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
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
