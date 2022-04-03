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
	void SetState(EItemState State) { ItemState = State; }
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FName ItemName = FName("Default");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount = 0;

	// Effects how many stars the pickup widget has
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity = EItemRarity::EIR_Common;
};
