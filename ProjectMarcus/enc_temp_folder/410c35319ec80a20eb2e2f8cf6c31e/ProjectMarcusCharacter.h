// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMarcusCharacter.generated.h"

#define LOCAL_USER_NUM 0

USTRUCT(BlueprintType)
struct FMoveData
{
	GENERATED_BODY()

	/* GAMEPAD SENSITIVITY */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float		GamepadTurnRate = 45.f;							// How fast character looks horizontally
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float		GamepadLookUpRate = 45.f;						// How fast character looks vertically
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float		GamepadAimingTurnRate = 20.f;					// How fast character looks horizontally when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float		GamepadAimingLookUpRate = 20.f;					// How fast character looks vertically when aiming

	/* MOUSE SENSITIVITY */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float		MouseTurnRate = 1.f;						// How fast character looks horizontally
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float		MouseLookUpRate = 1.f;						// How fast character looks vertically
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float		MouseAimingTurnRate = 0.2f;					// How fast character looks horizontally when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float		MouseAimingLookUpRate = 0.2f;				// How fast character looks vertically when aiming
		
	/* MOVEMENT */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float		JumpVelocity = 600.f;						// How high the character jumps

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float		AirControl = 0.5f;							// Higher value allows more air control

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FRotator	RotationRate = FRotator(0.f, 540.f, 0.f);	// Currently Unused. Determines how fast we rotate. lower = slow rotation. higher = fast. negative = snap instantly.
};

USTRUCT(BlueprintType)
struct FCameraData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float		DefaultFOV = 90.f;							// default FOV

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float		ZoomedFOV = 45.f;							// FOV while aiming

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float		BoomLength = 250.f;							// Camera arm length

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	FVector		ScreenOffset = FVector(0.f, 50.f, 50.f);	// Offsets from exact middle of screen

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	float		ZoomSpeed = 20.f;							// How fast we move into/out of zooming
};

UCLASS()
class PROJECTMARCUS_API AProjectMarcusCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectMarcusCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handles forwards/backwards input
	void MoveForward(float Value);

	// Handles side to side input
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate (effects yaw)
	 * @param Rate is a normalized rate, i.e. 1.0 = 100%, 0.5 = 50% of desired turn rate.
	 */
	void TurnAtRate_Gamepad(float Rate);

	/**
	 * Called via input to look up/down at a given rate (effects pitch)
	 * @param Rate is a normalized rate, i.e. 1.0 = 100%, 0.5 = 50% of desired look turn rate.
	 */
	void LookUpAtRate_Gamepad(float Rate);

	/**
	 * Rotate controller based off Mouse X movement
	 * @param Rate		The input value from mouse
	 */
	void TurnAtRate_Mouse(float Rate);

	/**
	 * Rotate controller based off Mouse Y movement
	 * @param Rate		The input value from mouse
	 */
	 void LookUpRate_Mouse(float Rate);

	void FireWeapon();

	void AimButtonPressed() { bIsAiming = true; }

	void AimButtonReleased() { bIsAiming = false; }

private:
	// Smoothly change camera FOV based off if the player is zooming or not
	void UpdateCameraZoom(float DeltaTime);

	void UpdateCurrentLookRate();

	// After firing a bullet get it's final impact point (either hits something or goes off infinitively far)
	// returns false only if there was an error during calculation
	bool GetBulletHitLocation(const FVector BarrelSocketLocation, FVector& OutHitLocation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FMoveData MoveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FCameraData CameraData;

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	// Camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCam;

	// Randomized gunshot sound cue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	// Flash spawned at BarrelSocket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	// Weapon fire montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	// Particles spawned at bullet impact point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletImpactParticles;

	// Smoke trail for bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletTrailParticles;

	bool bIsAiming = false;

	float CurrentFOV = 0.f;

	//TODO: Detect dynamically which input device (m/kb or gamepad) we're using and reflect that
	float CurrentGamepadTurnRate = 0.f;
	float CurrentGamepadLookUpRate = 0.f;
	float CurrentMouseTurnRate = 0.f;
	float CurrentMouseLookUpRate = 0.f;

public:
	FORCEINLINE USpringArmComponent* GetCameraArm() const { return CameraArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCam; }
	FORCEINLINE bool IsAiming() const { return bIsAiming; }
};
