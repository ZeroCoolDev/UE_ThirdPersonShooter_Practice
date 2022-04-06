#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectMarcusAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EAimOffsetState : uint8 
{
	EAOS_ADS UMETA(DisplayName = "Aim Down Sight"),
	EAOS_Hip UMETA(DisplayName = "Hip Placement"),
	EAOS_Reloading UMETA(DisplayName = "Reload"),
	EAOS_InAir UMETA(DisplayName = "InAir"),
	EAOS_Max UMETA(DisplayName = "InvalidMax")
};

/**
 * 
 */
UCLASS()
class PROJECTMARCUS_API UProjectMarcusAnimInstance : public UAnimInstance
{
	GENERATED_BODY()	

public:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime); // behaves much like tick function in actors instead of overriding NativeUpdateAnimationProperties

	virtual void NativeInitializeAnimation() override; // kinda like beginPlay for actors but for AnimInstances

protected:
	// Handles updating turning in place
	void CheckForTurnInPlace(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AProjectMarcusCharacter* PMCharacter;

	void FindOwner();

	// Speed of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float FootSpeed = 0.f;

	// Indicates if the character is in the air
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir = false;

	// Indicates if the character is accelerating (acceleration > 0)
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating = false;

	// Difference between [-180,180] the aim direction and movement direction. Used for strafing animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AimMovementDiff = 0.f;

	// strafing data in the last frame used for strafe ending animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastAimMovementDiff = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming = false;

	// Horizontal rotation difference between where the character is facing and the root is facing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	float YawDiffFromRootToCharacter = 0.f;

	// Threshold which needs to be passed before we actually turn in place
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	float TurnInPlaceYawThreshold = 90.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	float CurrentPitch = 0.f;

	// True when reloading, used to prevent hip aim offset while reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	bool bReloadingInProgress = false;

	// Used to determine which animation AimOffset to use (ADS or Hip or Reloading)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", meta = (AllowPrivateAccess = "true"))
	EAimOffsetState AimOffsetState = EAimOffsetState::EAOS_Hip;

	float CharacterYaw = 0.f;
	float CharacterYawLastFrame = 0.f;

	float RotationCurve = 0.f;
	float RotationCurveLastFrame = 0.f;
};
