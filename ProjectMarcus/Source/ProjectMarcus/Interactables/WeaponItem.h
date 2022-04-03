// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectMarcus/Interactables/ItemBase.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMARCUS_API AWeaponItem : public AItemBase
{
	GENERATED_BODY()

public:
	AWeaponItem();

	virtual void Tick(float DeltaTime) override;

	void SetState(EItemState State) { ItemState = State; }

	// Adds an impulse and rotation to the weapon
	void ThrowWeapon();

	void ConsumeAmmo(int32 Amt = 1);

	int32 GetAmmoCount() { return Ammo; }

protected:
	void StopFalling();

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo = 0;

private:
	FTimerHandle ThrowWeaponTimer;
	
	float ThrowDuration = 0.7f;
	
	bool bFalling = false;
};
