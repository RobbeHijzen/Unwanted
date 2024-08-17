// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBalloon.h"
#include "Kismet/GameplayStatics.h"

APlayerBalloon::APlayerBalloon()
{
	PrimaryActorTick.bCanEverTick = true;

	_BalloonCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Balloon Collision"));
	_OnSizeChanged.AddDynamic(this, &APlayerBalloon::AdjustCurrentSpeeds);
	_OnSizeChanged.AddDynamic(this, &APlayerBalloon::AdjustCollisionRadius);
}

void APlayerBalloon::BeginPlay()
{
	Super::BeginPlay();
	
	_OnSizeChanged.Broadcast();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_PlayerContext, 1);
		}
	}
}

void APlayerBalloon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(FVector{ 0.f, 0.f, 1.f } * _CurrentVerticalSpeed * DeltaTime, true);
}

void APlayerBalloon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind movement actions
		EnhancedInputComponent->BindAction(_StrafeAction, ETriggerEvent::Triggered, this, &APlayerBalloon::Strafe);
		EnhancedInputComponent->BindAction(_InflateAction, ETriggerEvent::Triggered, this, &APlayerBalloon::Inflate);
	}
}

void APlayerBalloon::Strafe(const FInputActionValue& value)
{
	float movementValue = value.Get<float>();
	AddActorWorldOffset(FVector{ 1.f, 0.f, 0.f } * movementValue * _CurrentHorizontalSpeed * GetWorld()->GetDeltaSeconds(), true);
}

void APlayerBalloon::Inflate(const FInputActionValue& value)
{
	float inflationValue = value.Get<float>();

	_CurrentRadius += GetWorld()->GetDeltaSeconds() * _InflationSpeed * inflationValue;

	// Clamp radius
	if (_CurrentRadius < _MinRadius)
	{
		_CurrentRadius = _MinRadius;
	}
	else if (_CurrentRadius > _MaxRadius)
	{
		_CurrentRadius = _MaxRadius;
	}
	else
	{
		_OnSizeChanged.Broadcast();
	}
}

void APlayerBalloon::AdjustCurrentSpeeds()
{
	float safeCurrentRadius{ FMath::Clamp(_CurrentRadius, _MinRadius, _MaxRadius) };
	float currentRadiusPercentage{ (safeCurrentRadius - _MinRadius) / (_MaxRadius - _MinRadius) };

	_CurrentVerticalSpeed = currentRadiusPercentage * (_MaxVerticalSpeed - _MinVerticalSpeed) + _MinVerticalSpeed;
	_CurrentHorizontalSpeed = (1 - currentRadiusPercentage) * (_MaxHorizontalSpeed - _MinHorizontalSpeed) + _MinHorizontalSpeed;
}

void APlayerBalloon::AdjustCollisionRadius()
{
	_BalloonCollision->SetSphereRadius(_CurrentRadius);
}

