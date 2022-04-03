
#include "ProjectMarcus/PlayerController/ProjectMarcusPlayerController.h"
#include "Blueprint/UserWidget.h"

AProjectMarcusPlayerController::AProjectMarcusPlayerController()
{

}

void AProjectMarcusPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayClass)
	{
		if (!HUDOverlay)
		{
			HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass, TEXT("HUDOverlay"));
			if (HUDOverlay)
			{
				HUDOverlay->AddToViewport();
				HUDOverlay->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}
