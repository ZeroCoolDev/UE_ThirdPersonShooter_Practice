#include "ProjectMarcus/Character/ProjectMarcusCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

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
			CameraArm->TargetArmLength = CameraData.BoomLength; // camera follows at this distance behind the character
			CameraArm->bUsePawnControlRotation = true; // rotate the arm based on the controller
			CameraArm->SocketOffset = CameraData.ScreenOffset;
	
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

	// Don't rotate the character when the controller rotates. We only want the controller to rotate the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// The character rotation is dependent on the movement component
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (ensure(MoveComp))
	{
		MoveComp->bOrientRotationToMovement = false; // Character moves in the direction of input
		MoveComp->JumpZVelocity = MoveData.JumpVelocity; // how high the character jumps
		MoveComp->AirControl = MoveData.AirControl; // 0 = no control. 1 = full control at max speed
	}
}

// Called every frame
void AProjectMarcusCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraZoom(DeltaTime);
	UpdateCurrentLookRate();

	CalculateCrosshairSpread(DeltaTime);
}

// Called to bind functionality to input
void AProjectMarcusCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (ensure(PlayerInputComponent))
	{
		// Movement
		PlayerInputComponent->BindAxis("MoveForward", this, &AProjectMarcusCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AProjectMarcusCharacter::MoveRight);

		// Look rotations
		PlayerInputComponent->BindAxis("TurnRate", this, &AProjectMarcusCharacter::TurnAtRate_Gamepad);
		PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectMarcusCharacter::LookUpAtRate_Gamepad);
		PlayerInputComponent->BindAxis("TurnMouse", this, &AProjectMarcusCharacter::TurnAtRate_Mouse);
		PlayerInputComponent->BindAxis("LookUpMouse", this, &AProjectMarcusCharacter::LookUpRate_Mouse);

		// Jump
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

		// Weapon Input
		PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &AProjectMarcusCharacter::FireWeapon);
		PlayerInputComponent->BindAction("AimButton", EInputEvent::IE_Pressed, this, &AProjectMarcusCharacter::AimButtonPressed);
		PlayerInputComponent->BindAction("AimButton", EInputEvent::IE_Released, this, &AProjectMarcusCharacter::AimButtonReleased);
	}
}

// Called when the game starts or when spawned
void AProjectMarcusCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FollowCam)
	{
		CameraData.DefaultFOV = FollowCam->FieldOfView;
		CurrentFOV = CameraData.DefaultFOV;
	}

	CurrentGamepadTurnRate = MoveData.GamepadTurnRate;
	CurrentGamepadLookUpRate = MoveData.GamepadLookUpRate;

	CurrentMouseTurnRate = MoveData.MouseAimingTurnRate;
	CurrentMouseLookUpRate = MoveData.MouseAimingLookUpRate;
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

		// Uses internal movement clamps of [0, 600]
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

		// Uses internal movement clamps of [0, 600]
		AddMovementInput(Direction, Value);
	}
}

void AProjectMarcusCharacter::TurnAtRate_Gamepad(float Rate)
{
	AddControllerYawInput(Rate * CurrentGamepadTurnRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame = deg/frame
}

void AProjectMarcusCharacter::LookUpAtRate_Gamepad(float Rate)
{
	AddControllerPitchInput(Rate * CurrentGamepadLookUpRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame = deg/frame
}

void AProjectMarcusCharacter::TurnAtRate_Mouse(float Rate)
{
	AddControllerYawInput(Rate * CurrentMouseTurnRate); // snap to position

}

void AProjectMarcusCharacter::LookUpRate_Mouse(float Rate)
{
	AddControllerPitchInput(Rate * CurrentMouseLookUpRate); // snap to position
}

void AProjectMarcusCharacter::FireWeapon()
{
	// SFX
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}

	// Muzzle Flash VFX + Linetracing/Collision + Impact Particles + Kickback Anim
	const USkeletalMeshComponent* CharMesh = GetMesh();
	if (CharMesh)
	{
		// Find the socket at the tip of the barrel with its current position and rotation and spawn a particle system
		const USkeletalMeshSocket* BarrelSocket = CharMesh->GetSocketByName("BarrelSocket");
		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(CharMesh);

			// Muzzle flash VFX
			if (MuzzleFlash)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
			}

			FVector BulletHitLocation;
			if (GetBulletHitLocation(SocketTransform.GetLocation(), BulletHitLocation))
			{

				if (GetWorld())
				{
					// Spawn impact particles
					if (BulletImpactParticles)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactParticles, BulletHitLocation);
					}

					// Spawn trail particles
					if (BulletTrailParticles)
					{
						UParticleSystemComponent* Trail = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTrailParticles, SocketTransform);
						if (Trail)
						{
							Trail->SetVectorParameter("Target", BulletHitLocation); // makes it so the particles appear in a line from TraceStart  to TrailEndPoint
						}
					}
				}
			}
		}

		// Play Kickback animation
		UAnimInstance* AnimInstance = CharMesh->GetAnimInstance();
		if (AnimInstance && HipFireMontage)
		{
			AnimInstance->Montage_Play(HipFireMontage);
			AnimInstance->Montage_JumpToSection("StartFire");
		}
	}
}

void AProjectMarcusCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	// Map from walk speed range to [0, 1]
	FVector2D WallkSpeedRange(0.f, 600.f);// default UE AddMovementInput range is [0, 600] which we are using
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f; // Must zero out the vertical velocity since this should only be effected by walking 
	float SpeedInWalkRange = Velocity.Size();
	// Low number when moving slowly, high number when moving quickly
	CrosshairVelocityFactor = (SpeedInWalkRange - WallkSpeedRange.X) / (WallkSpeedRange.Y - WallkSpeedRange.X);
	
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp && MoveComp->IsFalling())// TODO: IsFalling is not technically what I think I want to use
	{
		// Move further away slowly
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
	}
	else
	{
		// Move inwards very quickly
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
	}

	//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, FString::Printf(TEXT("\n\nCrosshairSpreadMultiplier = %f\nCrosshairVelocityFactor = %f\nCrosshairInAirFactor = %f"), CrosshairSpreadMultiplier, CrosshairVelocityFactor, CrosshairInAirFactor));
	CrosshairSpreadMultiplier = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor;
}

void AProjectMarcusCharacter::UpdateCameraZoom(float DeltaTime)
{
	// Just lerping by A + (B-A) * (t * Speed)
	if (bIsAiming)
	{
		if (CurrentFOV - CameraData.ZoomedFOV < SMALL_NUMBER)
		{// early bail if we're already where we need to be VS calling SetFieldOfView every frame...which is unnecessary
			return;
		}
		CurrentFOV = FMath::FInterpTo(CurrentFOV, CameraData.ZoomedFOV, DeltaTime, CameraData.ZoomSpeed);
	}
	else
	{
		if (CameraData.DefaultFOV - CurrentFOV < SMALL_NUMBER)
		{// early bail if we're already where we need to be VS calling SetFieldOfView every frame...which is unnecessary
			return;
		}
		CurrentFOV = FMath::FInterpTo(CurrentFOV, CameraData.DefaultFOV, DeltaTime, CameraData.ZoomSpeed);
	}

	FollowCam->SetFieldOfView(CurrentFOV);
}

void AProjectMarcusCharacter::UpdateCurrentLookRate()
{
	if (bIsAiming)
	{
		CurrentGamepadTurnRate = MoveData.GamepadAimingTurnRate;
		CurrentGamepadLookUpRate = MoveData.GamepadAimingLookUpRate;

		CurrentMouseTurnRate = MoveData.MouseAimingTurnRate;
		CurrentMouseLookUpRate = MoveData.MouseAimingLookUpRate;
	}
	else
	{
		CurrentGamepadTurnRate = MoveData.GamepadTurnRate;
		CurrentGamepadLookUpRate = MoveData.GamepadLookUpRate;

		CurrentMouseTurnRate = MoveData.MouseTurnRate;
		CurrentMouseLookUpRate = MoveData.MouseLookUpRate;
	}
}

bool AProjectMarcusCharacter::GetBulletHitLocation(const FVector BarrelSocketLocation, FVector& OutHitLocation)
{
	if (GetWorld())
	{
		// Get current viewport size
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// Get the cross hair local position (screen space)
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2); // Exact middle of the screen;
		CrosshairLocation.Y -= CameraData.ScreenOffset.Y; // adjust to match HUD

		// Translate crosshair to world position
		FVector CrosshairWorldPos;
		FVector CrosshairWorldDir;
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, LOCAL_USER_NUM),
			CrosshairLocation,
			CrosshairWorldPos,
			CrosshairWorldDir // Forward vector (so outwards from the viewport)
		);

		if (bScreenToWorld)
		{
			// Trace from crosshairs straight out
			FHitResult CrosshairsHit;
			const FVector CrosshairTraceStart(CrosshairWorldPos);
			const FVector CrosshairTraceEnd(CrosshairWorldPos + (CrosshairWorldDir * 50'000.f));
			OutHitLocation = CrosshairTraceEnd; // default hit location is as far as the trace went
			GetWorld()->LineTraceSingleByChannel(CrosshairsHit, CrosshairTraceStart, CrosshairTraceEnd, ECollisionChannel::ECC_Visibility);
			if (CrosshairsHit.bBlockingHit)
			{
				// Our crosshairs hit something, but still need to check if our bullet hit anything closer (possible unless we allow for changing aiming stance)
				OutHitLocation = CrosshairsHit.Location;
			}

			// Trace from weapon barrel socket to whatever the crosshairs hit
			FHitResult BulletHit;
			const FVector BulletTraceStart(BarrelSocketLocation);
			const FVector BulletTraceEnd(OutHitLocation);
			GetWorld()->LineTraceSingleByChannel(BulletHit, BulletTraceStart, BulletTraceEnd, ECollisionChannel::ECC_Visibility);
			if (BulletHit.bBlockingHit)
			{
				// Bullet hit something (might be the same as crosshairs, or something sooner)
				OutHitLocation = BulletHit.Location;
			}
			return true;			
		}
	}
	return false;
}

float AProjectMarcusCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

