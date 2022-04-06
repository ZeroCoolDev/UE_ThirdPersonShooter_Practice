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

		// Check if we are reloading or not
		bReloadingInProgress = PMCharacter->GetCombatState() == ECombatState::ECS_Reloading;

		// Order matters here.
		if (bReloadingInProgress)
		{
			AimOffsetState = EAimOffsetState::EAOS_Reloading;
		}
		else if(bIsInAir)
		{
			AimOffsetState = EAimOffsetState::EAOS_InAir;
		}
		else if (bIsAiming)
		{
			AimOffsetState = EAimOffsetState::EAOS_ADS;
		}
		else
		{
			AimOffsetState = EAimOffsetState::EAOS_Hip;
		}
		
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

	CheckForTurnInPlace(DeltaTime);
}

void UProjectMarcusAnimInstance::NativeInitializeAnimation()
{
	if (PMCharacter == nullptr)
	{
		FindOwner();
	}
}

void UProjectMarcusAnimInstance::CheckForTurnInPlace(float DeltaTime)
{
	// store the pitch (getting a rotation corresponding to the controller which matches our crosshairs)
	if (PMCharacter)
	{
		CurrentPitch = PMCharacter->GetBaseAimRotation().Pitch;
	}

	// for now don't allow turning in place while moving
	if (FootSpeed > 0 || bIsInAir)
	{
		YawDiffFromRootToCharacter = 0;
		CharacterYaw = PMCharacter->GetActorRotation().Yaw;
		CharacterYawLastFrame = CharacterYaw;
		RotationCurveLastFrame = 0.f;
		RotationCurve = 0.f;
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
		float turning = GetCurveValue(TEXT("Turning"));
		if (turning > 0.f)
		{
			// When the animation starts its first frame RotationCurve won't have a value, so setting  RotationCurveLastFrame = RotationCurve then subtracting them would essentially be 0-90
			// What we actually want is the delta between frames during the curve (which is a very small number like 89.5-90
			if (RotationCurveLastFrame == 0.f)
			{
				RotationCurve = GetCurveValue(TEXT("RotationV2"));
				RotationCurveLastFrame = RotationCurve;
			}
			else
			{
				RotationCurveLastFrame = RotationCurve;
				RotationCurve = GetCurveValue(TEXT("RotationV2"));
			}

			const float RotationCurveDelta = FMath::Abs(RotationCurveLastFrame - RotationCurve);

			// if YawDiffFromRootToCharacter is pos = turning left. is neg = turning right
			if (YawDiffFromRootToCharacter < 0.f) // turning right, we need to add (which decreases the amount we are compensating for turning) meaning we will start to turn the root to the direction we need
			{
				YawDiffFromRootToCharacter += RotationCurveDelta;
			}
			else
			{
				YawDiffFromRootToCharacter -= RotationCurveDelta; 
			}

			// Always clamp between our two maxima
			YawDiffFromRootToCharacter = FMath::Clamp(YawDiffFromRootToCharacter, -90.f, 90.f);
		}
	}
}

void UProjectMarcusAnimInstance::FindOwner()
{
	PMCharacter = Cast<AProjectMarcusCharacter>(TryGetPawnOwner());
}
