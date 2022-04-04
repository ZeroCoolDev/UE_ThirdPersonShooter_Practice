#include "ProjectMarcus/Character/ProjectMarcusAnimInstance.h"
#include "ProjectMarcus/Character/ProjectMarcusCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UProjectMarcusAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (PMCharacter == nullptr)
	{
		FindOwner();
	}

	if (PMCharacter)
	{
		// Update FootSpeed based off the lateral speed of the character from velocity (disregard vertical movement)
		FVector Velocity = PMCharacter->GetVelocity();
		Velocity.Z = 0.f; // we only want the lateral component of velocity so that if the character is falling or moving in a vertical way it doesn't effect our speed
		FootSpeed = Velocity.Size(); // the length of the velocity vector is the speed

		// Check if the character is in the air
		UCharacterMovementComponent* MoveComp = PMCharacter->GetCharacterMovement();
		if (ensure(MoveComp))
		{
			bIsInAir = MoveComp->IsFalling();
			bIsAccelerating = MoveComp->GetCurrentAcceleration().Size() > 0.f;
		}

		
		// What is the direction the controller is pointing in
		FRotator AimRotation = PMCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PMCharacter->GetVelocity()); // rotation from the world X direction based on some direction vector
		AimMovementDiff = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		// If we're moving, store the this frames movement so on the frame we stop moving, we know which direction we were headed to play the correct stop animation
		if (PMCharacter->GetVelocity().Size() > 0.f)
		{
			LastAimMovementDiff = AimMovementDiff;
		}

		bIsAiming = PMCharacter->IsAiming();
		
		// TODO: Swap this out for MovementOffset once you figure out how to indicate between left and right when DOT is 0...
		//FVector PlayerLookDir = PMCharacter->GetActorForwardVector().GetSafeNormal();
		//FVector MovementDir = PMCharacter->GetVelocity().GetSafeNormal();
		////AimMovementDiff = FVector::DotProduct(PlayerLookDir, MovementDir); 		
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, 
		//	FString::Printf(TEXT("Base Aim Rotation [%f]\nMovement Rotation [%f]\nAimMovementDiff [%f]\n\nPlayerLookDir (%f, %f)\nMovementDir (%f, %f)"), 
		//	AimRotation.Yaw, 
		//	MovementRotation.Yaw, 
		//	AimMovementDiff,
		//	PlayerLookDir.X,
		//	PlayerLookDir.Y, 
		//	MovementDir.X,
		//	MovementDir.Y));
		//}
	}
}

void UProjectMarcusAnimInstance::NativeInitializeAnimation()
{
	if (PMCharacter == nullptr)
	{
		FindOwner();
	}
}

void UProjectMarcusAnimInstance::FindOwner()
{
	PMCharacter = Cast<AProjectMarcusCharacter>(TryGetPawnOwner());
}
