// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class PROJECTMARCUS_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TogglePickupWidgetVisibility();

private:
	// Item Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh = nullptr;

	// Line trace collides with box to show HUD widgets
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox = nullptr;

	// Popup widget for when the player looks at the item (TODO: change it to be when with in rage)
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget = nullptr;
};
