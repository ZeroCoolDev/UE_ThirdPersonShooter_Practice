// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectMarcusPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMARCUS_API AProjectMarcusPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AProjectMarcusPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDOverlayClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* HUDOverlay;

};
