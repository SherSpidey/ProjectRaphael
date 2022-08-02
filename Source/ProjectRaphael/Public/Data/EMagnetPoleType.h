#pragma once

UENUM(BlueprintType)
enum class EMagnetPoleType: uint8
{
	EMT_North UMETA(DisplayName = "North"),
	EMT_South UMETA(DisplayName = "South"),
	EMT_Iron UMETA(DisplayName = "Iron"),
	EMT_Nonmagnetic UMETA(DisplayName = "Nonmagnetic"),
	
	EMT_MAX UMETA(DisplayName = "Default")
};