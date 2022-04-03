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

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	// Hide by default
	PickupWidget->SetVisibility(false);

	// Setup overlap for trigger
	ProximityTrigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnBeginOverlap);
	ProximityTrigger->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnEndOverlap);

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
		}
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::DeactivatePickupProperties()
{
	ProximityTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetVisibiity(false);
}

void AItemBase::SetVisibiity(bool bVisible)
{
	PickupWidget->SetVisibility(bVisible);
}

void AItemBase::UpdateToState(EItemState State)
{
	ItemState = State;

	switch (ItemState)
	{
	case EItemState::EIS_PickupWaiting: // Sitting on the ground waiting for someone to pick it up
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ProximityTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		ProximityTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	}
	case EItemState::EIS_EquipInterping:
		break;
	case EItemState::EIS_PickedUp:
		break;
	case EItemState::EIS_Equipped:
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ProximityTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ProximityTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PickupWidget->SetVisibility(false);
		break;
	}
	case EItemState::EIS_Drop:
	{
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		ItemMesh->DetachFromComponent(DetachmentRules);
		UpdateToState(EItemState::EIS_Falling);
		break;
	}
	case EItemState::EIS_Falling:
	{
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		ProximityTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ProximityTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PickupWidget->SetVisibility(false);
		break;
	}
	default:
		break;
	}
}

