#pragma once

UENUM(BlueprintType)
enum class EParticleState: uint8
{
	EPS_Idle UMETA(DisplayName = "Idle"),
	EPS_LoadOrUnLoad UMETA(DisplayName = "LoadOrUnload"),
	
	EPS_MAX UMETA(DisplayName = "Default")
};