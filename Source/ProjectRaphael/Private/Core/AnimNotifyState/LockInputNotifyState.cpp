// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AnimNotifyState/LockInputNotifyState.h"

#include "Character/BaseCharacter.h"

void ULockInputNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();
	if(Owner != nullptr)
	{
		ABaseCharacter* Player = Cast<ABaseCharacter>(Owner);
		if(Player)
		{
			Player->SetMovementEnable(false);
		}
	}
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void ULockInputNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if(Owner != nullptr)
	{
		ABaseCharacter* Player = Cast<ABaseCharacter>(Owner);
		if(Player)
		{
			Player->SetMovementEnable(true);
		}
	}
	
	Super::NotifyEnd(MeshComp, Animation);
}
