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

void AAmmoItem::BeginPlay()
{
	Super::BeginPlay();

}
