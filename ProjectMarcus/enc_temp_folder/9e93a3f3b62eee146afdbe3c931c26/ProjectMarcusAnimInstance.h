#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectMarcusAnimInstance.generated.h"

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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AProjectMarcusCharacter* PMCharacter;

	void FindOwner();

	// Speed of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float FootSpeed;

	// Indicates if the character is in the air
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	// Indicates if the character is accelerating (acceleration > 0)
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	// Difference between [-180,180] the aim direction and movement direction. Used for strafing animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float AimMovementDiff;

	// strafing data in the last frame used for strafe ending animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastAimMovementDiff;
};
