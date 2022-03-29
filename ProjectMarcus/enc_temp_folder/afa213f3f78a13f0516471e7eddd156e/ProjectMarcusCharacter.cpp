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
AProjectMarcusCharacter::AProjectMarcusCharacter() :
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f)
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
			CameraArm->SocketOffset = FVector(0.f, 50.f, 50.f); //offsets the character
	
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
		// TODO: move these into BP editable params for designers
		MoveComp->RotationRate = FRotator(0.f, 540.f, 0.f); // determines how fast we rotate. lower = slow rotation. higher = fast. negative = snap instantly
		MoveComp->JumpZVelocity = 600.f; // how high the character jumps
		MoveComp->AirControl = 0.5; // 0 = no control. 1 = full control at max speed
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

void AProjectMarcusCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame = deg/frame
}

void AProjectMarcusCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame = deg/frame
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

			if (MuzzleFlash)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
			}

			// Linetrace + Collision checking + Impact Particles
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
				CrosshairLocation.Y -= 50.f; // adjust to match HUD

				// Translate to world position
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
					FHitResult BulletTraceHit;
					const FVector BulletStart(CrosshairWorldPos);
					const FVector BulletEnd(CrosshairWorldPos + (CrosshairWorldDir * 50'000.f));

					// Default bullet trail will just go as far as the bullet if it hits nothing
					FVector BulletTrailEndPoint(BulletEnd);
					GetWorld()->LineTraceSingleByChannel(BulletTraceHit, BulletStart, BulletEnd, ECollisionChannel::ECC_Visibility);

					if (BulletTraceHit.bBlockingHit)
					{
						// Otherwise set it to exactly whatever we impacted with
						BulletTrailEndPoint = BulletTraceHit.Location;

						// Spawn impact particles
						if (BulletImpactParticles)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactParticles, BulletTraceHit.Location);
						}

						// Spawn trail particles
						if (BulletTrailParticles)
						{
							UParticleSystemComponent* Trail = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTrailParticles, BulletStart);
							if (Trail)
							{
								Trail->SetVectorParameter("Target", BulletTrailEndPoint); // makes it so the particles appear in a line from TraceStart  to TrailEndPoint
							}
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
		// Movement
		PlayerInputComponent->BindAxis("MoveForward", this, &AProjectMarcusCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AProjectMarcusCharacter::MoveRight);
		
		// Look rotations
		PlayerInputComponent->BindAxis("TurnRate", this, &AProjectMarcusCharacter::TurnAtRate);
		PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectMarcusCharacter::LookUpAtRate);
		PlayerInputComponent->BindAxis("TurnMouse", this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUpMouse", this, &APawn::AddControllerPitchInput);

		// Jump
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

		// Weapon
		PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &AProjectMarcusCharacter::FireWeapon);
	}
}

