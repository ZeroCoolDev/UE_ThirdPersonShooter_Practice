#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_9mm UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "Assault Rifle"),
	EAT_Max UMETA(DisplayName = "InvalidMax")
};