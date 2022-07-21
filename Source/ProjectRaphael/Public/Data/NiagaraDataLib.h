#pragma once

UENUM(BlueprintType)
enum class EParticleFunction: uint8
{
	EPF_Idle UMETA(DisplayName = "Idle"),
	EPF_Control UMETA(DisplayName = "Control"),
	
	EPF_MAX UMETA(DisplayName = "Default")
};