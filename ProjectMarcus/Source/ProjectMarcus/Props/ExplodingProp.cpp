#include "ProjectMarcus/Props/ExplodingProp.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AExplodingProp::AExplodingProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExplodingProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplodingProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplodingProp::OnBulletHit_Implementation(const FHitResult& HitResult)
{
	if (ExplodeSound)
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());

	if (ExplodeParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticles, HitResult.Location, FRotator(0.f), true);

	// TODO: Damage in AOE

	Destroy();
}

