// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectMarcus/Interactables/ItemBase.h"
#include "ProjectMarcus/AmmoType.h"
#include "AmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMARCUS_API AAmmoItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	AAmmoItem();

	virtual void Tick(float DeltaTime) override;

	EAmmoType GetAmmoType() { return AmmoType; }

	// Attempts to auto pickup based off current item state.
	// Will only pickup item if in the correct state to protect against multiple calls to pickup the same item
	void TryAutoPickup(float Distance);

protected:
	virtual void BeginPlay() override;

	virtual void UpdateToState(EItemState State) override;

private:
	// Mesh for the ammo pickup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AmmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType = EAmmoType::EAT_9mm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* IconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float AutoPickupDistance = 100.f;
};
