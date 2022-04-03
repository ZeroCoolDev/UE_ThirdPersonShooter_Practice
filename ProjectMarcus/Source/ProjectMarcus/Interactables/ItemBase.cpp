// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectMarcus/Interactables/ItemBase.h"
#include "ProjectMarcus/Character/ProjectMarcusCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

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
	CheckForItemPreviewInterp(DeltaTime);
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
		break;
	}
	case EItemState::EIS_PickUp:
	{
		// Mesh
		SetMeshVibility(true);
		DisableMeshPhysycs();
		
		// Pickup Trigger
		DisableProximityTrigger();

		// HUD
		SetPickupWidgetVisibility(false);

		StartPickupPreview();
		break;
	}
	case EItemState::EIS_PreviewInterping:
	{
		bPreviewInterping = true;
		break;
	}
	case EItemState::EIS_Equipped:
	{
		// Mesh
		SetMeshVibility(true);
		DisableMeshPhysycs();
		
		// Pickup Trigger
		DisableProximityTrigger();

		// HUD
		SetPickupWidgetVisibility(false);
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

		// HUD
		SetPickupWidgetVisibility(false);
		break;
	}
	default:
		break;
	}
}

void AItemBase::SetPickupWidgetVisibility(bool bVisible)
{
	PickupWidget->SetVisibility(bVisible);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	// Hide by default
	SetPickupWidgetVisibility(false);

	UpdateToState(EItemState::EIS_PickupWaiting);
}

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
				// Get current curve value
				const float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpHandle);
				const float CurveValue = ItemZPickupPreviewCurve->GetFloatValue(ElapsedTime);

				// Determine base Z height for when Curve is at 1.0
				const FVector CameraInterpLocation = CachedCharInPickupRange->GetCameraInterpLocation(); // Get location in front of the camera
				const FVector DistFromItemToCameraUp = FVector(0.f, 0.f, (CameraInterpLocation - ItemPickupPreviewStartLocation).Z); // Distance vertically between item's starting position and the camera
				const float BaseZHeight = DistFromItemToCameraUp.Size(); // Get a scalar for the desired Z height

				// CurveValue = 1.0 ItemLocationThisFrame will be exactly at DistFromItemToCameraUp. 
				// CurveValue > 1.0 ItemLocationThisFrame will be higher than DistFromItemToCameraUp
				// CurveValue < 1.0 ItemLocationThisFrame will be lower than DistFromItemToCameraUp
				const float ZHeightThisFrame = ItemPickupPreviewStartLocation.Z + (CurveValue * BaseZHeight);
				// Set the Z location of the item only
				FVector ItemLocationThisFrame = FVector(ItemPickupPreviewStartLocation.X, ItemPickupPreviewStartLocation.Y, ZHeightThisFrame);

				SetActorLocation(ItemLocationThisFrame, true, nullptr, ETeleportType::TeleportPhysics);
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

void AItemBase::StartPickupPreview()
{
	ItemPickupPreviewStartLocation = GetActorLocation();
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ItemInterpHandle, this, &AItemBase::FinishPickupPreview, ItemPickupPreviewDuration);
	}
	UpdateToState(EItemState::EIS_PreviewInterping);
}

void AItemBase::FinishPickupPreview()
{
	bPreviewInterping = false;
	if (CachedCharInPickupRange)
	{
		// Once we pick it up we turn off overlaps, so the organic "OnEndOverlap" which removes this item from range will not fire. Need to fire it manually on pickup
		CachedCharInPickupRange->RemoveItemInRange(this);
		CachedCharInPickupRange->PickupItemAfterPreview(this);
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

