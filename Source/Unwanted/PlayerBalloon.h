// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "PlayerBalloon.generated.h"

UCLASS()
class UNWANTED_API APlayerBalloon : public APawn
{
	GENERATED_BODY()

public:
	APlayerBalloon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* _PlayerContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* _StrafeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* _InflateAction;

	// Collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon")
	USphereComponent* _BalloonCollision{};


	// Balloon Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon Radius")
	float _MinRadius{50.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon Radius")
	float _MaxRadius{200.f};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon Speed")
	float _MinVerticalSpeed{500.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon Speed")
	float _MaxVerticalSpeed{1000.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon Speed")
	float _MinHorizontalSpeed{ 500.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon Speed")
	float _MaxHorizontalSpeed{ 1000.f };



	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Balloon Inflation")
	float _InflationSpeed{ 200.f };


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Balloon Inflation")
	float _CurrentRadius{50.f};
	float _CurrentVerticalSpeed{};
	float _CurrentHorizontalSpeed{};

	// Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSizeChanged);
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Balloon Radius")
	FOnSizeChanged _OnSizeChanged{};

private:	

	void Strafe(const FInputActionValue& value);
	void Inflate(const FInputActionValue& value);

	UFUNCTION()
	void AdjustCurrentSpeeds();
	UFUNCTION()
	void AdjustCollisionRadius();

	
};
