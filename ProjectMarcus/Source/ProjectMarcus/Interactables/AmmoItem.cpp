#include "ProjectMarcus/Interactables/AmmoItem.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

AAmmoItem::AAmmoItem()
{
	// create a custom root component
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	SetRootComponent(AmmoMesh);

	// Attach everything to it
	PickupWidget->SetupAttachment(GetRootComponent());
	ProximityTrigger->SetupAttachment(GetRootComponent());
}

void AAmmoItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoItem::TryAutoPickup(float Distance)
{
	if (Distance <= AutoPickupDistance && ItemState == EItemState::EIS_PickupWaiting)
	{
		UpdateToState(EItemState::EIS_PickUp);
	}
}

void AAmmoItem::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoItem::UpdateToState(EItemState State)
{
	Super::UpdateToState(State);

	switch (ItemState)
	{
	case EItemState::EIS_PickupWaiting:
	{
		// Set mesh visibility
		AmmoMesh->SetVisibility(true);

		// Disable mesh physics
		AmmoMesh->SetSimulatePhysics(false);
		AmmoMesh->SetEnableGravity(false);
		AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
	case EItemState::EIS_PickUp:
	{
		// Set mesh visibility
		AmmoMesh->SetVisibility(true);

		// Disable mesh physics
		AmmoMesh->SetSimulatePhysics(false);
		AmmoMesh->SetEnableGravity(false);
		AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
	case EItemState::EIS_PreviewInterping:
	{
		// nothing special
		break;
	}
	case EItemState::EIS_Equipped:
	{
		// Set mesh visibility
		AmmoMesh->SetVisibility(true);

		// Disable mesh physics
		AmmoMesh->SetSimulatePhysics(false);
		AmmoMesh->SetEnableGravity(false);
		AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
	case EItemState::EIS_Drop:
	{
		// Removes the item mesh from any component it was attached to (character mesh)
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		AmmoMesh->DetachFromComponent(DetachmentRules);
		break;
	}
	case EItemState::EIS_Falling:
	{
		// Set mesh visibility
		AmmoMesh->SetVisibility(true);

		// Enable mesh physics
		AmmoMesh->SetSimulatePhysics(true);
		AmmoMesh->SetEnableGravity(true);
		AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AmmoMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		AmmoMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		break;
	}
	default:
		break;
	}
}
