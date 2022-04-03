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

protected:
	void StopFalling();

private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowDuration = 0.7f;
	bool bFalling = false;
};
