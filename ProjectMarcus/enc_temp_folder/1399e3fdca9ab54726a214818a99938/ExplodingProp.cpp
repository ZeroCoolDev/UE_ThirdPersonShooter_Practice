#include "ProjectMarcus/Props/ExplodingProp.h"

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

