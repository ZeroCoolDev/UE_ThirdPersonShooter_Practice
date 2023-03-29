// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectMarcus/Interactables/ItemBase.h"
#include "ProjectMarcus/AmmoType.h"
#include "WeaponItem.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SubmachineGun UMETA(Display = "SubmachineGun"),
	EWT_AssaultRifle UMETA(Display = "AssaultRifle"),
	EWT_Max UMETA(Display = "InvalidMax")
};

// TODO: Use data table for weapon information

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

	void ReloadClip(int32 IncommingAmmo);

	int32 GetMaxAmmoCapacity() { return MaxClipCapacity; }

	int32 GetAmmoInClip() { return CurrentAmmoInClip; }

	EWeaponType GetWeaponType() { return WeaponType; }

	EAmmoType GetAmmoType() { return AmmoType; }

	const FName GetReloadMontage() { return ReloadMontageSection; }

	const FName GetClipBoneName() { return ClipBoneName; }

	void SetMovingClip(bool Moving) { bMovingClip = Moving; }

	float GetDamage() const { return Damage; }
	float GetHeadshotDamage() const { return HeadshotDamage; }


protected:
	void StopFalling();

	// Represents current ammo in the clip (0-AmmoClipCapacity)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmoInClip = 0;

	// Represents max ammo capacity for the clip for this ammo type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 MaxClipCapacity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType = EWeaponType::EWT_SubmachineGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType = EAmmoType::EAT_9mm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection = FName(TEXT("ReloadSMG"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName = FName(TEXT("smg_clip"));

	// Icon for this items ammo in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* AmmoIcon;

	// how much damage is applied on hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float HeadshotDamage;

private:
	FTimerHandle ThrowWeaponTimer;
	
	float ThrowDuration = 0.7f;
	
	bool bFalling = false;

	// True when moving the clip while reloading. TODO: Can I just use the combat state instead?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bMovingClip = false;
};
