// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "UnCommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),
	EIR_Max UMETA(DisplayName = "InvalidMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_PickupWaiting UMETA(DisplayName = "WaitingForPickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),
	EIR_Max UMETA(DisplayName = "InvalidMAX")
};

UCLASS()
class PROJECTMARCUS_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DeactivatePickupProperties();

	void SetVisibiity(bool bVisible);

	void UpdateToState(EItemState State);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Item Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh = nullptr;

	// Popup widget for when the player looks at the item (TODO: change it to be when with in rage)
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget = nullptr;

	// Detects if we are close enough to the pickup to perform vision checks (TODO: Which can also be done by the dot of our forward facing direction and the direction of the closest pickup)
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ProximityTrigger = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState = EItemState::EIS_PickupWaiting;
};
