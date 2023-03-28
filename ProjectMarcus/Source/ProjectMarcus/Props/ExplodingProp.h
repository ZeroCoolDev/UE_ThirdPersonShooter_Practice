// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectMarcus/Interfaces/BulletHitInterface.h"
#include "ExplodingProp.generated.h"

UCLASS()
class PROJECTMARCUS_API AExplodingProp : public AActor, public IBulletHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplodingProp();

	// Particles to spawn when hit by bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta= (AllowPrivateAccess = true))
	class UParticleSystem* ExplodeParticles;

	// Sound to play when hit by bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta= (AllowPrivateAccess = true))
	class USoundCue* ExplodeSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnBulletHit_Implementation(const FHitResult& HitResult) override;
};
