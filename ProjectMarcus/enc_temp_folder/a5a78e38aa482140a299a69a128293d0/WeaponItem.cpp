// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectMarcus/Interactables/WeaponItem.h"



AWeaponItem::AWeaponItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFalling && ItemState == EItemState::EIS_Falling)
	{
		if (ItemMesh)
		{
			// Keeps the weapon horizontally level in the world (upright)
			FRotator MeshCurrentRotation(0.f, ItemMesh->GetComponentRotation().Yaw, 0.f);
			ItemMesh->SetWorldRotation(MeshCurrentRotation, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}

void AWeaponItem::ThrowWeapon()
{
	if (ItemMesh)
	{
		FRotator HorizontallyLevelRotation(0.f, ItemMesh->GetComponentRotation().Yaw, 0.f); // keeps the weapon level and right side up
		ItemMesh->SetWorldRotation(HorizontallyLevelRotation, false, nullptr, ETeleportType::TeleportPhysics);

		FVector MeshForward = ItemMesh->GetForwardVector();
		FVector ImpulseDir = ItemMesh->GetRightVector();
		
		// Angle 20 degrees down
		ImpulseDir = ImpulseDir.RotateAngleAxis(-20.f, MeshForward);

		// Angle random degrees left or right (around the up vector)
		float RandomRotation = FMath::RandRange(15.f, 45.f);
		ImpulseDir = ImpulseDir.RotateAngleAxis(RandomRotation, FVector::UpVector);

		// Arbitrarily large number so the impulse in the world is obvious
		ImpulseDir *= 20'000.f;

		// Launch it
		ItemMesh->AddImpulse(ImpulseDir);
		bFalling = true;
	}
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeaponItem::StopFalling, ThrowDuration);
	}
}

void AWeaponItem::StopFalling()
{
	bFalling = false;
	UpdateToState(EItemState::EIS_PickupWaiting);
}
