#include "ProjectMarcus/Character/ProjectMarcusCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AProjectMarcusCharacter::AProjectMarcusCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (CameraArm == nullptr)
	{
		CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
		if (ensure(CameraArm))
		{
			// Create a camera boom (pulls in towards the character if there is a collision)
			CameraArm->SetupAttachment(RootComponent);
			CameraArm->TargetArmLength = 300.f; // camera follows at this distance behind the character
			CameraArm->bUsePawnControlRotation = true; // rotate the arm based on the controller
	
			if (FollowCam == nullptr)
			{
				FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
				if (ensure(FollowCam))
				{
					FollowCam->SetupAttachment(CameraArm, USpringArmComponent::SocketName); // attach camera to end of arm
					FollowCam->bUsePawnControlRotation = false; // don't want the camera to rotate relative to arm - the camera follows the camera arms rotation
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AProjectMarcusCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectMarcusCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		// Get the rotation around the up (z) axis, find fwd vector, move in that direction
		const FRotator Rotation(Controller->GetControlRotation());
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // yaw = rotation around up/z axis

		// find out the direction the controller is pointing fwd
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X)); // EAxis::X = forward direction

		AddMovementInput(Direction, Value);
	}
}

void AProjectMarcusCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		// Get the rotation around the up (z) axis, find right vector, move in that direction
		const FRotator Rotation(Controller->GetControlRotation());
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // yaw = rotation around up/z axis

		// find out the direction the controller is pointing right
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y)); // EAxis::Y = right direction

		AddMovementInput(Direction, Value);
	}
}

// Called every frame
void AProjectMarcusCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AProjectMarcusCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (ensure(PlayerInputComponent))
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &AProjectMarcusCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AProjectMarcusCharacter::MoveRight);
	}
}

