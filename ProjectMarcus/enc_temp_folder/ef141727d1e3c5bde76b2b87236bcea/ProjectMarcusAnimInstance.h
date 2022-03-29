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

	// Offset Yaw used for strafing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffset;

	// Difference between aim direction and movement direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AimMovementDiff;
};
