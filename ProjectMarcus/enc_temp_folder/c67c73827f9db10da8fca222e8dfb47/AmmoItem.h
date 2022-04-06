// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectMarcus/Interactables/ItemBase.h"
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

protected:
	virtual void BeginPlay() override;

private:
	// Mesh for the ammo pickup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AmmoMesh;
};
