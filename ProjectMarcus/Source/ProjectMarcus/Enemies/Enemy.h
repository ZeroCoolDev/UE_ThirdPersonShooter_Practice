// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMarcus/Interfaces/BulletHitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECTMARCUS_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void Die();

	void PlayHitMontage(FName Section, float PlayRate = 1.f);

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector HitLocation);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta= (AllowPrivateAccess = true))
	class UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta= (AllowPrivateAccess = true))
	class USoundCue* ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	FString HeadBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float HealthBarDisplayTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	FTimerHandle HealthBarTimer;

	// Contains hit and death animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float HitReactIntervalMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float HitReactIntervalMax;

	FTimerHandle HitReactTimer;

	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = true))
	TMap<UUserWidget*, FVector> HitNumbers;

	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = true))
	float HitNumberLifetime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnBulletHit_Implementation(const FHitResult& HitResult) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE const FString& GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation);
};
