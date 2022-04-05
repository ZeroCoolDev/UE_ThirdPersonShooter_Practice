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

	CheckForTurnInPlace();
}

void UProjectMarcusAnimInstance::NativeInitializeAnimation()
{
	if (PMCharacter == nullptr)
	{
		FindOwner();
	}
}

void UProjectMarcusAnimInstance::CheckForTurnInPlace()
{
	// for now don't allow turning in place while moving
	if (FootSpeed > 0)
	{
		return;
	}

	if(PMCharacter)
	{
		CharacterYawLastFrame = CharacterYaw;
		CharacterYaw = PMCharacter->GetActorRotation().Yaw;
		const float CharacterYawDelta = CharacterYaw - CharacterYawLastFrame;
		
		// Clamped to [-180, 180]
		YawDiffFromRootToCharacter = UKismetMathLibrary::NormalizeAxis(YawDiffFromRootToCharacter - CharacterYawDelta);
		
		// This will only be true if the animation playing has the Turning metadata 
		bool Turning = GetCurveValue(TEXT("Turning")) > 0.f;
		if (Turning)
		{
			RotationCurveLastFrame = RotationCurve;// -90
			RotationCurve = GetCurveValue(TEXT("RotationV2"));// 0 = -89
			ensure((int32)RotationCurve == 90);
			GEngine->AddOnScreenDebugMessage(0, -1, FColor::Red, FString::Printf(TEXT("RotationCurve: %f"), RotationCurve));
			const float RotationCurveDelta = RotationCurve - RotationCurveLastFrame; // -89 + 90 = 1

			// if YawDiffFromRootToCharacter is pos = turning left. is neg = turning right
			if (YawDiffFromRootToCharacter < 0) // turning right, we need to add (which decreases the amount we are compensating for turning) meaning we will start to turn the root to the direction we need
			{
				YawDiffFromRootToCharacter += RotationCurveDelta;
				//GEngine->AddOnScreenDebugMessage(0, -1, FColor::Red, FString::Printf(TEXT("Updating YawDiffFromRootToCharacter += [RotationCurveDelta = RotationCurve - RotationCurveLastFrame](%f = %f - %f)"), RotationCurveDelta, RotationCurve, RotationCurveLastFrame));
			}
			else
			{
				YawDiffFromRootToCharacter -= RotationCurveDelta; 
				//GEngine->AddOnScreenDebugMessage(0, -1, FColor::Blue, FString::Printf(TEXT("Updating YawDiffFromRootToCharacter -= %f"), RotationCurveDelta));
			}

			//const float AbsYawDiff = FMath::Abs(YawDiffFromRootToCharacter);
			//if(AbsYawDiff > TurnInPlaceYawThreshold)
			//{
			//	const float YawExcess = AbsYawDiff - TurnInPlaceYawThreshold;
			//	YawDiffFromRootToCharacter > 0 ? TurnInPlaceYawThreshold -= YawExcess : TurnInPlaceYawThreshold += YawExcess;
			//}
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, -1, FColor::White, FString::Printf(TEXT("CharacterYaw: %f"), CharacterYaw));
			GEngine->AddOnScreenDebugMessage(2, -1, FColor::Green, FString::Printf(TEXT("YawDiffFromRootToCharacter: %f"), YawDiffFromRootToCharacter));
		}
	}
}

void UProjectMarcusAnimInstance::FindOwner()
{
	PMCharacter = Cast<AProjectMarcusCharacter>(TryGetPawnOwner());
}
