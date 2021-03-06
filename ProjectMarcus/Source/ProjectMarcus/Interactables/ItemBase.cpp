// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectMarcus/Interactables/ItemBase.h"
#include "ProjectMarcus/Character/ProjectMarcusCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveVector.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make the mesh the root
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());

	ProximityTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityTrigger"));
	ProximityTrigger->SetupAttachment(GetRootComponent());
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Interp position for preview pickup
	CheckForItemPreviewInterp(DeltaTime);
	
	// Get curve values from pulse curve and set dynamic material params
	UpdatePulseCurveValues();
}

void AItemBase::UpdateToState(EItemState State)
{
	ItemState = State;

	switch (ItemState)
	{
	case EItemState::EIS_PickupWaiting:
	{
		// Mesh
		SetMeshVibility(true);
		DisableMeshPhysycs();
		
		// Pickup Trigger
		EnableProximityTrigger();
		
		// Once we enter pickup waiting, kickoff the pulse timer
		ResetPulseTimer();

		// Once we drop turn the glow back on
		SetGlowMaterial(true);
		break;
	}
	case EItemState::EIS_PickUp:
	{
		// should only do this is not ammo?
		PickupLocationIdx = CachedCharInPickupRange->AddItemToPickupLocation();

		// Pickup SFX
		PlayPickupSound();

		// Mesh
		SetMeshVibility(true);
		DisableMeshPhysycs();
		
		// Pickup Trigger
		DisableProximityTrigger();

		// HUD
		// Only disable pickup widget, keep the glow/colour materials until we equip it
		SetPickupWidgetVisibility(false);

		StartPickupPreview();
		break;
	}
	case EItemState::EIS_PreviewInterping:
	{
		bPreviewInterping = true;
		// We want to keep these on while interping (they will get turned off once equipped)
		SetCustomDepth(true);
		break;
	}
	case EItemState::EIS_PickedUpNoEquip:
	{
		// TODO: If we want to play a sound on pickup but no equip put that here
		//PlayEquipSound();

		// Mesh
		SetMeshVibility(true);
		DisableMeshPhysycs();

		// Pickup Trigger
		DisableProximityTrigger();

		// HUD & VFX
		// fully disable all visuals (pickup widget, glow and outline materials)
		SetPickupItemVisuals(false);

		// Just safety net turning it off
		SetGlowMaterial(false);

		break;
	}
	case EItemState::EIS_Equipped:
	{
		// Equip SFX
		PlayEquipSound();

		// Mesh
		SetMeshVibility(true);
		DisableMeshPhysycs();
		
		// Pickup Trigger
		DisableProximityTrigger();

		// HUD & VFX
		// fully disable all visuals (pickup widget, glow and outline materials)
		SetPickupItemVisuals(false);

		// Just safety net turning it off
		SetGlowMaterial(false);
		break;
	}
	case EItemState::EIS_Drop:
	{
		// Removes the item mesh from any component it was attached to (character mesh)
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		ItemMesh->DetachFromComponent(DetachmentRules);

		// Move directly into falling state
		UpdateToState(EItemState::EIS_Falling);
		break;
	}
	case EItemState::EIS_Falling:
	{
		// Mesh
		SetMeshVibility(true);
		EnableMeshPhysics();

		// Pickup Trigger
		DisableProximityTrigger();

		// HUD & VFX
		SetPickupItemVisuals(false);
		break;
	}
	default:
		break;
	}
}

void AItemBase::SetPickupItemVisuals(bool bIsVisible)
{
	SetCustomDepth(bIsVisible);
	// Doing the opposite means we glow when NOT being looked at, and not when we're looked at
	SetGlowMaterial(!bIsVisible);
	SetPickupWidgetVisibility(bIsVisible);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	// Hide by default
	SetPickupItemVisuals(false);

	UpdateToState(EItemState::EIS_PickupWaiting);

	// disable custom depth by default
	InitCustomDepth();

	if (ItemState == EItemState::EIS_PickupWaiting)
	{

	}
}

void AItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (BaseMaterialInstance)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterialInstance, this, TEXT("ItemDynamicMatInst"));
		if (DynamicMaterialInstance)
		{
			ItemMesh->SetMaterial(MaterialIndex, DynamicMaterialInstance);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AItemBase::OnConstruction, Failed to create dynamic material instance!"));
		}
	}

	SetGlowMaterial(true);
}

void AItemBase::InitCustomDepth()
{
	SetCustomDepth(false);
}

void AItemBase::SetCustomDepth(bool bEnabled)
{
	if (ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(bEnabled);
	}
}

void AItemBase::SetGlowMaterial(bool bEnabled)
{
	if (DynamicMaterialInstance)
	{
		
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowBlendAlpha"), (int)bEnabled);
	}
}

void AItemBase::SetPickupWidgetVisibility(bool bVisible)
{
	PickupWidget->SetVisibility(bVisible);
}

// TODO: Technically if the character spawns within the overlap region this "on begin" doesn't trigger since they didn't _enter_ the overlap.
// Might want to check something like once a second if something is inside us. Maybe not. Something to consider. Maybe this is only possible in the editor
void AItemBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// tell the actor that just started overlapping that we are within range
	if (OtherActor)
	{
		// check if the actor overlapping is our character
		AProjectMarcusCharacter* PMCharacter = Cast<AProjectMarcusCharacter>(OtherActor);
		if (PMCharacter)
		{
			PMCharacter->AddItemInRange(this);
			CachedCharInPickupRange = PMCharacter;
		}
	}
}

void AItemBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// tell the actor that just ended overlapping we are no longer in range
	if (OtherActor)
	{
		// check if the actor overlapping is our character
		AProjectMarcusCharacter* PMCharacter = Cast<AProjectMarcusCharacter>(OtherActor);
		if (PMCharacter)
		{
			PMCharacter->RemoveItemInRange(this);
			CachedCharInPickupRange = nullptr;
		}
	}
}

void AItemBase::CheckForItemPreviewInterp(float DeltaTime)
{
	if (bPreviewInterping)
	{
		if (CachedCharInPickupRange)
		{
			if (ItemZPickupPreviewCurve)
			{
				const float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpHandle);

				/* Calculate Location */

				const float ZPositionCurveValue = ItemZPickupPreviewCurve->GetFloatValue(ElapsedTime);

				// Determine base Z height for when Curve is at 1.0
				FVector CameraInterpLocation;
				GetPickupInterpLocation(CameraInterpLocation); //CachedCharInPickupRange->GetCameraInterpLocation(); // Get location in front of the camera

				const FVector DistFromItemToCameraUp = FVector(0.f, 0.f, (CameraInterpLocation - ItemPickupPreviewStartLocation).Z); // Distance vertically between item's starting position and the camera
				const float BaseZHeight = DistFromItemToCameraUp.Size(); // Get a scalar for the desired Z height

				// CurveValue = 1.0 ItemLocationThisFrame will be exactly at DistFromItemToCameraUp. 
				// CurveValue > 1.0 ItemLocationThisFrame will be higher than DistFromItemToCameraUp
				// CurveValue < 1.0 ItemLocationThisFrame will be lower than DistFromItemToCameraUp
				const float ZHeightThisFrame = ItemPickupPreviewStartLocation.Z + (ZPositionCurveValue * BaseZHeight);

				const FVector ItemCurrentLocation = GetActorLocation();
				// Val = A + (B-A) * (t * speed)
				const float InterpXValue = FMath::FInterpTo(ItemCurrentLocation.X, CameraInterpLocation.X, DeltaTime, 30.f);
				const float InterpYValue = FMath::FInterpTo(ItemCurrentLocation.Y, CameraInterpLocation.Y, DeltaTime, 30.f);

				// New location based off curve and X/Y interpolation
				FVector ItemLocationThisFrame = FVector(InterpXValue, InterpYValue, ZHeightThisFrame);
				SetActorLocation(ItemLocationThisFrame, true, nullptr, ETeleportType::TeleportPhysics);

				/* Calculate Rotation */

				UCameraComponent* CharCamComp = CachedCharInPickupRange->GetFollowCamera();
				if (CharCamComp)
				{
					// Pickup Visual v1
					// Item is rotated to match the cameras rotation but keeps all item rotations in tact. Player sees item in rotation matching when it was picked up, but always in front of the camera.
					//const FRotator ItemOffsetCameraYaw = FRotator(0.f, CharCamComp->GetComponentRotation().Yaw + YawDiffBetweenCameraAndItem,0.f);
					//SetActorRotation(ItemOffsetCameraYaw, ETeleportType::TeleportPhysics);

					// Pickup Visual v2
					// Item matches camera Yaw only. Player sees side face of the item matching camera Yaw rotation, but with original Pitch and Roll.
					//const FRotator ItemMatchCameraYaw = FRotator(0.f, CharCamComp->GetComponentRotation().Yaw, 0.f);
					//SetActorRotation(ItemMatchCameraYaw, ETeleportType::TeleportPhysics);

					// Pickup Visual v3 (current)
					// Item matches camera rotation exactly. Player sees the side face of the item matching camera rotation
					SetActorRotation(CharCamComp->GetComponentRotation(), ETeleportType::TeleportPhysics);
				}

				/* Calculate Scale */
				if (ItemScaleCurve)
				{
					// CurveValue = 1.0 most of the curve till it decreases sharply at the end (shrinking at the end)
					const float ScaleCurveValue = ItemScaleCurve->GetFloatValue(ElapsedTime);
					SetActorScale3D(FVector(ScaleCurveValue, ScaleCurveValue, ScaleCurveValue));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ItemZPickupPreviewCurve is invalid!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CachedCharInPickupRange is invalid!"));
		}
	}
}

void AItemBase::GetPickupInterpLocation(FVector& OutInterpLocation)
{
	OutInterpLocation = FVector::ZeroVector;

	if (CachedCharInPickupRange)
	{
		switch (ItemType)
		{
			case EItemType::EIT_Ammo:
			{
				CachedCharInPickupRange->GetPickupLocationLocation(PickupLocationIdx, OutInterpLocation);
				break;
			}
			case EItemType::EIT_Weapon:
			{
				CachedCharInPickupRange->GetPickupLocationLocation(0, OutInterpLocation); // the 0th index is always our weapon index
				break;
			}
			default:
				UE_LOG(LogTemp, Error, TEXT("AItemBase::GetPickupInterpLocation, EItemType not set!"));
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CachedCharacter is invalid!"));
	}
}

void AItemBase::StartPickupPreview()
{
	ItemPickupPreviewStartLocation = GetActorLocation();
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ItemInterpHandle, this, &AItemBase::FinishPickupPreview, ItemPickupPreviewDuration);

		// No need to run the pulse timer if we're picked up
		GetWorld()->GetTimerManager().ClearTimer(PulseTimer);
	}

	// Store the angle between camera and item (so we know what constant angle offset to keep the item at relative to the camera if the player rotates during pickup)
	if (CachedCharInPickupRange)
	{
		// Remove the item we're picking up immediately from the range list so we don't show the widget in mid preview
		CachedCharInPickupRange->RemoveItemInRange(this);

		UCameraComponent* CharCamComponent = CachedCharInPickupRange->GetFollowCamera();
		if (CharCamComponent)
		{
			const float CameraYawRotation = CharCamComponent->GetComponentRotation().Yaw;
			const float ItemYawRotataion = GetActorRotation().Yaw;

			YawDiffBetweenCameraAndItem = ItemYawRotataion - CameraYawRotation;
		}
	}

	UpdateToState(EItemState::EIS_PreviewInterping);
}

void AItemBase::FinishPickupPreview()
{
	bPreviewInterping = false;
	// reset scale of mesh since we shrunk it during preview
	SetActorScale3D(FVector(1.f));
	if (CachedCharInPickupRange)
	{
		// Tell the character we're not longer interping to pickup location
		CachedCharInPickupRange->RemoveItemFromPickupLocation(PickupLocationIdx);

		// Once we pick it up we turn off overlaps, so the organic "OnEndOverlap" which removes this item from range will not fire. Need to fire it manually on pickup
		CachedCharInPickupRange->PickupItemAfterPreview(this);
	}

	if (ItemType == EItemType::EIT_Ammo)
	{
		PlayEquipSound();
	}
}

void AItemBase::EnableProximityTrigger()
{
	if (ProximityTrigger)
	{
		if (!ProximityTrigger->OnComponentBeginOverlap.IsBound())
		{
			ProximityTrigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnBeginOverlap);
		}
		if (!ProximityTrigger->OnComponentEndOverlap.IsBound())
		{
			ProximityTrigger->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnEndOverlap);
		}
	}

	ProximityTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ProximityTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AItemBase::DisableProximityTrigger()
{
	if (ProximityTrigger)
	{
		if (ProximityTrigger->OnComponentBeginOverlap.IsBound())
		{
			ProximityTrigger->OnComponentBeginOverlap.RemoveDynamic(this, &AItemBase::OnBeginOverlap);
		}
		if (ProximityTrigger->OnComponentEndOverlap.IsBound())
		{
			ProximityTrigger->OnComponentEndOverlap.RemoveDynamic(this, &AItemBase::OnEndOverlap);
		}
	}

	ProximityTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProximityTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemBase::EnableMeshPhysics()
{
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetEnableGravity(true);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AItemBase::DisableMeshPhysycs()
{
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetEnableGravity(false);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemBase::SetMeshVibility(bool bVisible)
{
	ItemMesh->SetVisibility(bVisible);
}

void AItemBase::PlayPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
	}
}

void AItemBase::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySound2D(this, EquipSound);
	}
}

void AItemBase::UpdatePulseCurveValues()
{
	float ElapsedTime;
	FVector CurveValue;

	// Difference pulses based off whether its being actively picked up or not
	switch (ItemState)
	{
		case EItemState::EIS_PickupWaiting:
			if (PulseCurve)
			{
				ElapsedTime = GetWorldTimerManager().GetTimerElapsed(PulseTimer);
				CurveValue = PulseCurve->GetVectorValue(ElapsedTime);
			}
		break;
		case EItemState::EIS_PreviewInterping:
			if (InterpPulseCurve)
			{
				ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpHandle);
				CurveValue = InterpPulseCurve->GetVectorValue(ElapsedTime);
			}
		break;
	}


	const float GlowAmtThisFrame = CurveValue.X;
	const float FresnelExpThisFrame = CurveValue.Y;
	const float FresnelReflectThisFrame = CurveValue.Z;

	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowAmount"), GlowAmtThisFrame * GlowMatAlpha);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("FresnelExponent"), FresnelExpThisFrame * FresnelExponent);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("FresnelReflectFraction"), FresnelReflectThisFrame * FresnelReflect);
	}
}

void AItemBase::ResetPulseTimer()
{
	if (ItemState == EItemState::EIS_PickupWaiting)
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(PulseTimer, this, &AItemBase::ResetPulseTimer, PulseCurveDuration);
		}
	}
}

