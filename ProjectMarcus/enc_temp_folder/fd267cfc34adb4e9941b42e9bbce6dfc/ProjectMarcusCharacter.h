// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMarcusCharacter.generated.h"

#define LOCAL_USER_NUM 0

USTRUCT()
struct FMoveData
{
	GENERATED_BODY()
	float		TurnRate = 45.f;
	float		LookUpRate = 45.f;
	float		JumpVelocity = 600.f;						// How high the character jumps
	float		AirControl = 0.5f;							// Higher value allows more air control
	FRotator	RotationRate = FRotator(0.f, 540.f, 0.f);	// Might be obsolete
};

USTRUCT()
struct FCameraData
{
	GENERATED_BODY()
	float		DefaultFOV = 90.f;
	float		ZoomedFOV = 45.f;
	float		BoomLength = 250.f;							// Camera arm length
	FVector		ScreenOffset = FVector(0.f, 50.f, 50.f);	// Offsets from exact middle of screen
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
	void TurnAtRate(float Rate);

	/**
	 * Called via input to look up/down at a given rate (effects pitch)
	 * @param Rate is a normalized rate, i.e. 1.0 = 100%, 0.5 = 50% of desired look turn rate.
	 */
	void LookUpAtRate(float Rate);

	void FireWeapon();

	void AimButtonPressed();

	void AimButtonReleased();

private:
	bool GetFinalHitLocation(const FVector BarrelSocketLocation, FVector& OutHitLocation);

	FMoveData MoveData;
	FCameraData CameraData;

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	// Camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCam;

	// Base turn rate in deg/sec. Other scaling may effect final turn rate (pressure sensitivity of the thumbstick for ex)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 1.f;

	// Base look up/down rate in deg/sec. Other scaling may effect final turn rate (pressure sensitivity of the thumbstick for ex)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate = 1.f;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	float CameraDefaultFOV;

	float CameraZoomedFOV;

public:
	FORCEINLINE USpringArmComponent* GetCameraArm() const { return CameraArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCam; }
};
