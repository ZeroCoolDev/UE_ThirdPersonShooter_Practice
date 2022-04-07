#include "ProjectMarcus/Character/ProjectMarcusCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectMarcus/Interactables/WeaponItem.h"
#include "ProjectMarcus/Interactables/AmmoItem.h"

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

	// Setup scene component for attaching weapon magazine
	if(HandSceneComponent == nullptr)
	{
		HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComponent"));
	}

	// Setup scene components for picking up items interpolation
	if (WeaponInterpComp == nullptr)
	{
		WeaponInterpComp = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponInterpComp"));
		WeaponInterpComp->SetupAttachment(GetFollowCamera());
		PickupLocations.Add(FPickupInterpLocationData(WeaponInterpComp, 0));
	}

	FString InterpCompNameBase = "PickupInterpLoc";
	for (uint8 i = 0; i < 6; ++i)
	{
		// Create unique name since UE requires this
		FName InterpCompName = FName(InterpCompNameBase + FString::FromInt(i));
		
		// Attach to camera
		USceneComponent* PickupLoc = PickupInterpLocations.Add_GetRef(CreateDefaultSubobject<USceneComponent>(InterpCompName));
		PickupLoc->SetupAttachment(GetFollowCamera());

		// Store in the list of possible locations for in game use
		PickupLocations.Add(FPickupInterpLocationData(PickupLoc, 0));
	}

}

// Called every frame
void AProjectMarcusCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraZoom(DeltaTime);
	UpdateCurrentLookRate();

	CalculateCrosshairSpread(DeltaTime);

	CheckForItemsInRange();
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
		PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Pressed, this, &AProjectMarcusCharacter::FireButtonPressed);
		PlayerInputComponent->BindAction("FireButton", EInputEvent::IE_Released, this, &AProjectMarcusCharacter::FireButtonReleased);
		PlayerInputComponent->BindAction("AimButton", EInputEvent::IE_Pressed, this, &AProjectMarcusCharacter::AimButtonPressed);
		PlayerInputComponent->BindAction("AimButton", EInputEvent::IE_Released, this, &AProjectMarcusCharacter::AimButtonReleased);

		// Selection
		PlayerInputComponent->BindAction("Select", EInputEvent::IE_Pressed, this, &AProjectMarcusCharacter::SelectButtonPressed);
		PlayerInputComponent->BindAction("Select", EInputEvent::IE_Released, this, &AProjectMarcusCharacter::SelectButtonReleased);
		
		// Reload
		PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AProjectMarcusCharacter::ReloadButtonPressed);
	}
}

void AProjectMarcusCharacter::AddItemInRange(AItemBase* ItemInRange)
{
	if (ItemInRange)
	{
		ItemsInRange.FindOrAdd(ItemInRange->GetUniqueID(), MakeWeakObjectPtr<AItemBase>(ItemInRange));
	}
}

void AProjectMarcusCharacter::RemoveItemInRange(AItemBase* ItemOutOfRange)
{
	if (ItemOutOfRange)
	{
		ItemOutOfRange->SetPickupWidgetVisibility(false);
		ItemsInRange.Remove(ItemOutOfRange->GetUniqueID());
		// If the item we are no longer in range of was our currently focused item, remove it
		if (CurrentlyFocusedItem && ItemOutOfRange->GetUniqueID() == CurrentlyFocusedItem->GetUniqueID())
		{
			CurrentlyFocusedItem = nullptr;
		}
	}
}

// Deprecated - TODO: remove
//FVector AProjectMarcusCharacter::GetCameraInterpLocation()
//{
//	// Defaulting to 0 so if something is wrong we're very aware
//	FVector ItemPreviewLoc = FVector::ZeroVector;
//
//	if (GetFollowCamera())
//	{
//		const FVector CameraWorldPos = GetFollowCamera()->GetComponentLocation();
//		const FVector CamForwardDir = GetFollowCamera()->GetForwardVector();
//		// desired = camLoc + forward * A + up * B
//		ItemPreviewLoc = CameraWorldPos + 
//						 CamForwardDir * CameraData.ItemPickupDistranceOut + 
//						 FVector(0.f, 0.f, CameraData.ItemPickupDistanceUp);
//	}
//	return ItemPreviewLoc;
//}

void AProjectMarcusCharacter::PickupItemAfterPreview(AItemBase* PickedupItem)
{
	if (AWeaponItem* WeaponItem = Cast<AWeaponItem>(PickedupItem))
	{
		SwapWeapon(WeaponItem);
	}
	
	if (AAmmoItem* AmmoItem = Cast<AAmmoItem>(PickedupItem))
	{
		PickupAmmo(AmmoItem);
	}
}

int32 AProjectMarcusCharacter::GetAmmoStashForType(EAmmoType AmmoType)
{
	if (AmmoStashMap.Contains(AmmoType))
	{
		return AmmoStashMap[AmmoType];
	}
	return -1;
}

void AProjectMarcusCharacter::GetPickupLocationLocation(int32 LocationIndex, FVector& OutPickupLocation)
{
	OutPickupLocation = FVector::ZeroVector;

	if (LocationIndex < PickupLocations.Num())
	{
		USceneComponent* PickupLoc = PickupLocations[LocationIndex].PickupLoc;
		if(PickupLoc)
		{
			OutPickupLocation = PickupLoc->GetComponentLocation();
		}
	}
}

int32 AProjectMarcusCharacter::AddItemToPickupLocation()
{
	int32 LeastFilledLoc = GetLeastFilledPickupLocation();
	PickupLocations[LeastFilledLoc].AddItem();
	return LeastFilledLoc;
}

void AProjectMarcusCharacter::RemoveItemFromPickupLocation(int32 LocationIndex)
{
	if (LocationIndex < PickupLocations.Num())
	{
		PickupLocations[LocationIndex].RemoveItem();
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

	EquipWeapon(SpawnDefaultWeapon());

	CurrentGamepadTurnRate = MoveData.GamepadTurnRate;
	CurrentGamepadLookUpRate = MoveData.GamepadLookUpRate;

	CurrentMouseTurnRate = MoveData.MouseAimingTurnRate;
	CurrentMouseLookUpRate = MoveData.MouseAimingLookUpRate;

	FillAmmoStash();
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

void AProjectMarcusCharacter::SelectButtonPressed()
{
	if (CurrentlyFocusedItem)
	{
		CurrentlyFocusedItem->UpdateToState(EItemState::EIS_PickUp);
	}
}

void AProjectMarcusCharacter::SelectButtonReleased()
{

}

void AProjectMarcusCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}

void AProjectMarcusCharacter::FireWeapon()
{
	if (EquippedWeapon == nullptr)
	{
		return;
	}

	if (CombatState != ECombatState::ECS_Unoccupied)
	{// We can only fire if we're doing nothing else
		return;
	}

	if (WeaponClipHasAmmo())
	{
		PlayBulletFireSfx();
		SendBulletWithVfx();
		ApplyWeaponKickback();
		StartCrosshairBulletFire();
		EquippedWeapon->ConsumeAmmo();
		StartFireTimer();
	}
	else
	{// If they tried firing but have nothing in the clip, reload for them
		ReloadWeapon();
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
	
	// Calculate Crosshair Aim factor
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp && MoveComp->IsFalling())// TODO: IsFalling is not technically what I think I want to use
	{// Move further away slowly
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
	}
	else
	{// Move inwards very quickly
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
	}

	if (bIsAiming)
	{// Move inwards very quickly
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, -0.6f, DeltaTime, 30.f);
	}
	else
	{// Move outwards very quickly
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
	}

	CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, bIsFiringBullet ? 0.3f : 0.f, DeltaTime, 60.f);

	//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, FString::Printf(TEXT("\n\nCrosshairSpreadMultiplier = %f\nCrosshairVelocityFactor = %f\nCrosshairInAirFactor = %f"), CrosshairSpreadMultiplier, CrosshairVelocityFactor, CrosshairInAirFactor));
	CrosshairSpreadMultiplier = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor + CrosshairAimFactor + CrosshairShootingFactor;
}

void AProjectMarcusCharacter::StartCrosshairBulletFire()
{
	bIsFiringBullet = true;

	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(ShootTimeHandle, this, &AProjectMarcusCharacter::FinishCrosshairBulletFire, ShootTimeDuration);
	}
}

void AProjectMarcusCharacter::FinishCrosshairBulletFire()
{
	bIsFiringBullet = false;
}

void AProjectMarcusCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	FireWeapon();
}

void AProjectMarcusCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AProjectMarcusCharacter::AutoFireReset()
{
	CombatState = ECombatState::ECS_Unoccupied;

	if (WeaponClipHasAmmo())
	{
		if (bFireButtonPressed)
		{
			FireWeapon();
		}
	}
	else
	{
		ReloadWeapon();
	}
}

AWeaponItem* AProjectMarcusCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		if (GetWorld())
		{
			// Spawn the weapon
			return Cast<AWeaponItem>(GetWorld()->SpawnActor<AWeaponItem>(DefaultWeaponClass));
		}
	}
	return nullptr;
}

void AProjectMarcusCharacter::EquipWeapon(AWeaponItem* NewWeapon)
{
	if (NewWeapon) // TODO: We really gotta split the logic and classes between the weapon PICKUP item, and a weapon you actually use in the game
	{
		// Get the mesh
		USkeletalMeshComponent* SkeletalMesh = GetMesh();
		if (SkeletalMesh)
		{
			const USkeletalMeshSocket* HandSocket = SkeletalMesh->GetSocketByName(FName("RightHandSocket"));
			if (HandSocket)
			{
				// Attach the weapon to the hand on the mesh
				HandSocket->AttachActor(NewWeapon, SkeletalMesh);
			}
		}

		EquippedWeapon = NewWeapon;
		EquippedWeapon->UpdateToState(EItemState::EIS_Equipped);
	}
}

void AProjectMarcusCharacter::DropWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->UpdateToState(EItemState::EIS_Drop);
		EquippedWeapon->ThrowWeapon();
	}
}

void AProjectMarcusCharacter::SwapWeapon(AWeaponItem* WeaponToSwap)
{
	DropWeapon();
	EquipWeapon(WeaponToSwap);
}

void AProjectMarcusCharacter::PickupAmmo(class AAmmoItem* Ammo)
{
	if (Ammo)
	{
		// Add however many bullets this ammo pickup has to our stash
		AddAmmoToStash(Ammo->GetAmmoType(), Ammo->GetItemCount());

		// Check is we can automatically reload given the new amount of ammo
		if (EquippedWeapon)
		{
			if (EquippedWeapon->GetAmmoType() == Ammo->GetAmmoType() && EquippedWeapon->GetAmmoInClip() == 0)
			{
				ReloadWeapon();
			}
		}
	}

	//TODO: Dont do this...
	Ammo->Destroy();
}

void AProjectMarcusCharacter::RemoveAmmoFromStash(EAmmoType AmmoType, int32 RemovedAmmo)
{
	ensure(AmmoStashMap.Contains(AmmoType));
	if (AmmoStashMap.Find(AmmoType))
	{
		int32 CurAmmoForType = AmmoStashMap[AmmoType];
		AmmoStashMap[AmmoType] = FMath::Max(CurAmmoForType - RemovedAmmo, 0);
	}
}

void AProjectMarcusCharacter::AddAmmoToStash(EAmmoType AmmoType, int32 AddedAmmo)
{
	ensure(AmmoStashMap.Contains(AmmoType));
	if (AmmoStashMap.Find(AmmoType))
	{
		int32 CurAmmoForType = AmmoStashMap[AmmoType];
		AmmoStashMap[AmmoType] = FMath::Min(CurAmmoForType + AddedAmmo, 999);
	}
}

void AProjectMarcusCharacter::FillAmmoStash()
{
	AmmoStashMap.Empty();
	AmmoStashMap = { {EAmmoType::EAT_9mm, Starting9mmAmmo}, {EAmmoType::EAT_AR, StartingARAmmo} };
}

void AProjectMarcusCharacter::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unoccupied)
	{
		return;
	}

	if (EquippedWeapon)
	{
		// Only allow reloading if we don't have a full clip
		if (EquippedWeapon->GetAmmoInClip() < EquippedWeapon->GetMaxAmmoCapacity())
		{
			if (CarryingAmmoTypeForCurrentWeapon())
			{
				CombatState = ECombatState::ECS_Reloading;

				if (ReloadMontage)
				{
					USkeletalMeshComponent* MeshComp = GetMesh();
					if (MeshComp)
					{
						UAnimInstance* AnimInst = MeshComp->GetAnimInstance();
						if (AnimInst)
						{
							AnimInst->Montage_Play(ReloadMontage);
							AnimInst->Montage_JumpToSection(EquippedWeapon->GetReloadMontage());
						}
					}
				}
			}
		}
	}
}

void AProjectMarcusCharacter::FinishReloading()
{
	if (EquippedWeapon)
	{
		EAmmoType CurAmmoType = EquippedWeapon->GetAmmoType();
		if (AmmoStashMap.Contains(CurAmmoType))
		{
			int32 AmmoPutIntoClip = 0;
			int32 EmptySpaceInClip = EquippedWeapon->GetMaxAmmoCapacity() - EquippedWeapon->GetAmmoInClip();

			if (EmptySpaceInClip > AmmoStashMap[CurAmmoType])
			{// Can only fill clip with the few bullets we had in the stash
				AmmoPutIntoClip = AmmoStashMap[CurAmmoType];
			}
			else
			{// Fill entire clip
				AmmoPutIntoClip = EmptySpaceInClip;
			}

			EquippedWeapon->ReloadClip(AmmoPutIntoClip);
			RemoveAmmoFromStash(CurAmmoType, AmmoPutIntoClip);
		}
	}

	// In case the user is still holding the fire button during reload
	AutoFireReset();
}

void AProjectMarcusCharacter::GrabClip()
{
	if (EquippedWeapon)
	{
		USkeletalMeshComponent* WeaponMesh = EquippedWeapon->GetItemMesh();
		if (WeaponMesh)
		{
			int32 ClipBoneIndex = WeaponMesh->GetBoneIndex(EquippedWeapon->GetClipBoneName());
			ClipTransform = WeaponMesh->GetBoneTransform(ClipBoneIndex);

			if (HandSceneComponent)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true); // KeepRelative - because we want to keep relative location from the hand to the clip
				// HandSceneComponent will follow the position of Hand_L during any animations
				HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules, FName("Hand_L"));
				// The clip bone will follow the hand compnent, which is following the animation
				HandSceneComponent->SetWorldTransform(ClipTransform);
			}

			EquippedWeapon->SetMovingClip(true);
		}
	}
}

void AProjectMarcusCharacter::ReleaseClip()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetMovingClip(false);
	}
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

void AProjectMarcusCharacter::CheckForItemsInRange()
{
	if (ItemsInRange.Num())
	{
		// Get where the player is actually "looking" (where the crosshairs are outwards)
		// not where the PlayerController is facing (because they're offset to the side) wo we can't use GetActorForwardLocation()
		FVector CrosshairLocationInWorld;
		FVector CrosshairDirectionInWorld;
		GetCrosshairWorldPosition(CrosshairLocationInWorld, CrosshairDirectionInWorld);
		FVector LookDir = CrosshairLocationInWorld + CrosshairDirectionInWorld * TRACE_FAR;

		for (auto It = ItemsInRange.CreateConstIterator(); It; ++It)
		{
			AItemBase* Item = Cast<AItemBase>(It->Value);
			if (Item)
			{
				// Must use crosshair location again vs GetActorLocation() because we want to know the difference in LOOK vectors, not position vectors
				FVector DirToThisItem = Item->GetActorLocation() - CrosshairLocationInWorld;
				float LookingAtItemAmount = FVector::DotProduct(LookDir.GetSafeNormal(), DirToThisItem.GetSafeNormal());
				// If the look vector and direction to item is close, toggle the popup visible, otherwise toggle it off
				if (LookingAtItemAmount >= ItemPopupVisibilityThreshold)
				{
					Item->SetPickupWidgetVisibility(true);
					// Regardless if one was set already, update the currently focused item to the latest one looking at
					CurrentlyFocusedItem = Item;
				}
				else
				{
					// If the item we are no longer looking at was our currently focused item, remove it
					if (CurrentlyFocusedItem && Item->GetUniqueID() == CurrentlyFocusedItem->GetUniqueID())
					{
						CurrentlyFocusedItem = nullptr;
					}
					Item->SetPickupWidgetVisibility(false);
				}

				// If this is ammo try to auto pick it up
				if (AAmmoItem* AmmoItem = Cast<AAmmoItem>(Item))
				{
					FVector DirFromPlayerToItem = AmmoItem->GetActorLocation() - GetActorLocation();
					float DistanceToItem = DirFromPlayerToItem.Size();

					DrawDebugLine(GetWorld(), GetActorLocation(), AmmoItem->GetActorLocation(), FColor::Green);
					GEngine->AddOnScreenDebugMessage(1, -1, FColor::Green, FString::Printf(TEXT("DistanceToItem: %f"), DistanceToItem));

					AmmoItem->TryAutoPickup(DistanceToItem);
				}
			}
		}
	}
}

void AProjectMarcusCharacter::PlayBulletFireSfx()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
}

void AProjectMarcusCharacter::SendBulletWithVfx()
{
	// Muzzle Flash VFX + Linetracing/Collision + Impact Particles + Kickback Anim
	const USkeletalMeshComponent* WeaponMesh = EquippedWeapon->GetItemMesh();
	if (WeaponMesh)
	{
		// Find the socket at the tip of the barrel with its current position and rotation and spawn a particle system
		const USkeletalMeshSocket* BarrelSocket = WeaponMesh->GetSocketByName("BarrelSocket");
		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(WeaponMesh);

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
	}
}

void AProjectMarcusCharacter::ApplyWeaponKickback()
{
	// Play Kickback animation
	const USkeletalMeshComponent* CharMesh = GetMesh();
	if (CharMesh)
	{
		UAnimInstance* AnimInstance = CharMesh->GetAnimInstance();
		if (AnimInstance && HipFireMontage)
		{
			AnimInstance->Montage_Play(HipFireMontage);
			AnimInstance->Montage_JumpToSection("StartFire");
		}
	}
}

void AProjectMarcusCharacter::StartFireTimer()
{
	CombatState = ECombatState::ECS_FireTimerInProgress;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimeHandle, this, &AProjectMarcusCharacter::AutoFireReset, AutomaticFireRate);
	}
}

bool AProjectMarcusCharacter::GetBulletHitLocation(const FVector BarrelSocketLocation, FVector& OutHitLocation)
{
	if (GetWorld())
	{
		FHitResult CrosshairHitResult;
		TraceFromCrosshairs(CrosshairHitResult, OutHitLocation);

		// Trace from weapon barrel socket to whatever the crosshairs hit
		FHitResult BulletHit;
		const FVector BulletTraceStart = BarrelSocketLocation;

		const FVector StartToEnd = OutHitLocation - BulletTraceStart; // Direction Vector from start to end
		const FVector BulletTraceEnd = BarrelSocketLocation + StartToEnd * 1.25f; // OutHitLocation increased further by 25%
		GetWorld()->LineTraceSingleByChannel(BulletHit, BulletTraceStart, BulletTraceEnd, ECollisionChannel::ECC_Visibility); 
		if (BulletHit.bBlockingHit)
		{
			// Bullet hit something (might be the same as crosshairs, or something sooner)
			OutHitLocation = BulletHit.Location;
		}
		return true;
	}

	return false;
}

bool AProjectMarcusCharacter::TraceFromCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	if (GEngine)
	{
		FVector CrosshairLocationInWorld;
		FVector CrosshairDirectionInWorld;
		if (GetCrosshairWorldPosition(CrosshairLocationInWorld, CrosshairDirectionInWorld))
		{
			// 	Trace from crosshair pos in world outwards (in crosshair direction in world)
			const FVector TraceStart = CrosshairLocationInWorld;
			const FVector TraceEnd = CrosshairLocationInWorld + (CrosshairDirectionInWorld * TRACE_FAR);
			OutHitLocation = TraceEnd;
			if (GetWorld())
			{
				GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
				if (OutHitResult.bBlockingHit)
				{
					OutHitLocation = OutHitResult.Location;
					return true;
				}
			}
		}
	}

	return false;
}

bool AProjectMarcusCharacter::GetCrosshairWorldPosition(FVector& OutWorldPos, FVector& OutWorldDir)
{
	// Get viewport size
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	// Get screen space location of crosshairs
	FVector2D CrosshairLocationOnScreen(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	CrosshairLocationOnScreen.Y -= CameraData.ScreenOffset.Y; // need to match offset in HUD

	// Get world location and direction of crosshairs
	return UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, LOCAL_USER_NUM), CrosshairLocationOnScreen, OutWorldPos, OutWorldDir);
}

bool AProjectMarcusCharacter::CarryingAmmoTypeForCurrentWeapon()
{
	if (EquippedWeapon)
	{
		EAmmoType CurAmmoType = EquippedWeapon->GetAmmoType();
		if (AmmoStashMap.Contains(CurAmmoType))
		{
			return AmmoStashMap[CurAmmoType] > 0;
		}
	}
	return false;
}

int32 AProjectMarcusCharacter::GetLeastFilledPickupLocation()
{
	int32 MinNum = INT_MAX;
	int32 IdxWithLeast = 1;
	for (uint8 i = 1, End = PickupLocations.Num(); i < End; ++i)
	{
		if (PickupLocations[i].NumItemsInterping < MinNum)
		{
			MinNum = PickupLocations[i].NumItemsInterping;
			IdxWithLeast = i;
		}
	}
	return IdxWithLeast;
}

float AProjectMarcusCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

bool AProjectMarcusCharacter::WeaponClipHasAmmo()
{
	if (EquippedWeapon)
	{
		return EquippedWeapon->GetAmmoInClip() > 0;
	}
	return false;
}

