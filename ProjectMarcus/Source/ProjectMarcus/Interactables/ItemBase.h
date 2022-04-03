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
	EIS_PickUp UMETA(DisplayName = "PickUp"),
	EIS_PreviewInterping UMETA(DisplayName = "PreviewInterping"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Drop	UMETA(DisplayName = "Drop"),
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

	void UpdateToState(EItemState State);

	void SetPickupWidgetVisibility(bool bVisible);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// handles item interpolation when in the EIS_PreviewInterping state
	void CheckForItemPreviewInterp(float DeltaTime);

	void StartPickupPreview();
	void FinishPickupPreview();

	void EnableProximityTrigger();
	void DisableProximityTrigger();

	void EnableMeshPhysics();
	void DisableMeshPhysycs();

	void SetMeshVibility(bool bVisible);

	// Item Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh = nullptr;

	// Popup widget for when the player looks at the item (TODO: change it to be when with in rage)
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget = nullptr;

	// Detects if we are close enough to the pickup to perform vision checks (TODO: Which can also be done by the dot of our forward facing direction and the direction of the closest pickup)
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ProximityTrigger = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FName ItemName = FName("Default");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount = 0;

	// Effects how many stars the pickup widget has
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity = EItemRarity::EIR_Common;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState = EItemState::EIS_PickupWaiting;

	// The curve asset to use for the items Z location when interping on pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZPickupPreviewCurve;

	/* Item Pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemPickupPreviewStartLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemPickupPreviewEndLocation = FVector::ZeroVector;
	
	bool bPreviewInterping = false;
	
	FTimerHandle ItemInterpHandle;

	// Duration matches the curve length
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float ItemPickupPreviewDuration = 0.7f;

	// I don't like this - why do we need a reference to our character. WE should just be given a target location and interp to there
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AProjectMarcusCharacter* CachedCharInPickupRange = nullptr;
};
