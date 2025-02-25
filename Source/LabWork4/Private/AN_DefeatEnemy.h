// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_DefeatEnemy.generated.h"

/**
 * 
 */
UCLASS()
class LABWORK4_API UAN_DefeatEnemy : public UAnimNotify
{
	GENERATED_BODY()
public:

	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
