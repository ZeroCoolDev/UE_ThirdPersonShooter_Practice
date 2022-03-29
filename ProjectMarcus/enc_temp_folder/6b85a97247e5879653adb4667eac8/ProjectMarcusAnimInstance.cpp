#include "ProjectMarcus/Character/ProjectMarcusAnimInstance.h"
#include "ProjectMarcus/Character/ProjectMarcusCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
