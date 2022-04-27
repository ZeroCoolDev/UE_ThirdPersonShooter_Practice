// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMarcus/AmmoType.h"
#include "ProjectMarcusCharacter.generated.h"

#ifndef LOCAL_USER_NUM
#define LOCAL_USER_NUM 0
#endif

#ifndef TRACE_FAR
#define TRACE_FAR 50'000.f
#endif //TRACE_FAR

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float		ItemPickupDistranceOut = 250.f;				// Distance outwards from the cameras forward vector where the item previews on pickup

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float		ItemPickupDistanceUp = 65.f;				// Distance upwards from the camera where the item previews on pickup
};

UENUM(BlueprintType) 
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FiringInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_Max UMETA(DisplayName = "InvalidMax")
};

USTRUCT(BlueprintType)
struct FPickupInterpLocationData
{
	GENERATED_BODY()

	FPickupInterpLocationData() : FPickupInterpLocationData(nullptr, 0){}
	FPickupInterpLocationData(class USceneComponent* InPickupLoc, int32 InNum) :
	PickupLoc(InPickupLoc),
	NumItemsInterping(InNum){}

	void AddItem()
	{
		++NumItemsInterping;
	}

	void RemoveItem() 
	{
		if (NumItemsInterping > 0)
		{
			--NumItemsInterping;
		}
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* PickupLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NumItemsInterping;
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

	void AddItemInRange(class AItemBase* ItemInRange);

	void RemoveItemInRange(class AItemBase* ItemOutOfRange);

	// Deprecated - TODO: remove
	//FVector GetCameraInterpLocation();

	void PickupItemAfterPreview(AItemBase* PickedupItem);

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoStashForType(EAmmoType AmmoType);

	ECombatState GetCombatState() const { return CombatState; }

	// Get the location in world space for the requested PickupLocation
	void GetPickupLocationLocation(int32 LocationIndex, FVector& OutPickupLocation);

	// Finds the lowest filled pickup location, adds one to it's count, and returns the index
	int32 AddItemToPickupLocation();

	// Removes an item from the pickup location at LocationIndex
	void RemoveItemFromPickupLocation(int32 LocationIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Input / Locomotion */

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

	void SelectButtonPressed();

	void SelectButtonReleased();

	void ReloadButtonPressed();

	/* Camera Zoom / Sensitivity */

	void AimButtonPressed() { bIsAiming = true; }

	void AimButtonReleased() { bIsAiming = false; }

	/* Crosshair movement */
	
	void CalculateCrosshairSpread(float DeltaTime);
	
	UFUNCTION()
	void StartCrosshairBulletFire();
	
	UFUNCTION()
	void FinishCrosshairBulletFire();

	/* Weapon Fire */
	void FireWeapon();

	void FireButtonPressed();

	void FireButtonReleased();

	UFUNCTION()
	void AutoFireReset();

	class AWeaponItem* SpawnDefaultWeapon();

	// Attaches the given weapon to our character mesh
	void EquipWeapon(class AWeaponItem* NewWeapon);

	void DropWeapon();

	// Drops currently equipped weapon, and equips whatever weapon is currently being looked at
	void SwapWeapon(class AWeaponItem* WeaponToSwap);

	void PickupAmmo(class AAmmoItem* Ammo);

	void RemoveAmmoFromStash(EAmmoType AmmoType, int32 RemovedAmmo);

	void AddAmmoToStash(EAmmoType AmmoType, int32 AddedAmmo);

	void FillAmmoStash();

	/* Reload Weapon */
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void GrabClip();

	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

private:
	// Smoothly change camera FOV based off if the player is zooming or not
	void UpdateCameraZoom(float DeltaTime);

	void UpdateCurrentLookRate();

	void CheckForItemsInRange();

	void PlayBulletFireSfx();

	void SendBulletWithVfx();

	void ApplyWeaponKickback();

	void StartFireTimer();

	// After firing a bullet get it's final impact point (either hits something or goes off infinitively far)
	// returns false only if there was an error during calculation
	bool GetBulletHitLocation(const FVector BarrelSocketLocation, FVector& OutHitLocation);

	// Line trace from crosshairs (in world space). OutHitResult contains a hit if one occurred. OUtHitLocation contains the ending trace location whether it hit something or not.
	bool TraceFromCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	bool GetCrosshairWorldPosition(FVector& OutWorldPos, FVector& OutWorldDir);

	// Checks if the weapon's clip has ammo
	bool WeaponClipHasAmmo();

	// Checks if we have the weapon types ammo
	bool CarryingAmmoTypeForCurrentWeapon();

	int32 GetLeastFilledPickupLocation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FMoveData MoveData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	// Particles spawned at bullet impact point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletImpactParticles;

	// Smoke trail for bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletTrailParticles;

	/* Crosshairs */

	// Determines the spread
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier = 0.f;

	// Velocity component for spread. Low number when moving slowly, high number when moving quickly [0, maxWalkSpeed]
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor = 0.f;

	// In air component for spread
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor = 0.f;

	// Aim component for spread
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor = 0.f;

	// Shooting component for spread
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Crosshair, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor = 0.f;

	// Keeps track of our current weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AWeaponItem* EquippedWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponItem> DefaultWeaponClass;

	// Keeps track of the item the player is currently looking at (used for knowing which item we want to actually pickup and swap with)
	// Will be nullptr if not looking at anything
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AItemBase* CurrentlyFocusedItem = nullptr;

	// TODO: Use a fixed length array instead
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoStashMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	// Transform of the clip when we grab it during reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;

	// Scene compoonent we attach to the characters hand during reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* HandSceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Interp Points", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* WeaponInterpComp = nullptr;

	// List of scene components for BP editing 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Interp Points", meta = (AllowPrivateAccess = "true"))
	TArray<class USceneComponent*> PickupInterpLocations;

	// List of actual pickup location data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Interp Points", meta = (AllowPrivateAccess = "true"))
	TArray<FPickupInterpLocationData> PickupLocations;

private:
	/* fire related */

	// Used for crosshair animation while firing
	float ShootTimeDuration = 0.05f;
	bool bIsFiringBullet = false;
	FTimerHandle ShootTimeHandle;

	// Used for automatic firing
	bool bFireButtonPressed = false;
	//bool bShouldFire = true;
	float AutomaticFireRate = 0.1f; // automatic weapon fire rate (fire/second) - needs to be larger than ShootTimeDuration
	FTimerHandle AutoFireTimeHandle;

	// Used for zooming the camera in/out when aiming
	bool bIsAiming = false;
	float CurrentFOV = 0.f;

	//TODO: Detect dynamically which input device (m/kb or gamepad) we're using and reflect that
	float CurrentGamepadTurnRate = 0.f;
	float CurrentGamepadLookUpRate = 0.f;
	float CurrentMouseTurnRate = 0.f;
	float CurrentMouseLookUpRate = 0.f;

	// Cache of any items we are in range of 
	UPROPERTY()
	TMap<uint32, TWeakObjectPtr<class AItemBase>> ItemsInRange;
	// Threshold for how close the player needs to look at (1 = directly at it, 0.5 = 50% between looking and not...etc)
	float ItemPopupVisibilityThreshold = 0.99f;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<AItemBase*> Inventory;
	const int INVENTORY_CAPACITY = 6;

public:
	FORCEINLINE USpringArmComponent* GetCameraArm() const { return CameraArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCam; }
	FORCEINLINE bool IsAiming() const { return bIsAiming; }
	
	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;
};
