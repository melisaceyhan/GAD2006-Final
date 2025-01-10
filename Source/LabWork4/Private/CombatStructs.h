// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatStructs.generated.h"

USTRUCT(BlueprintType)
struct FAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString sName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* pAttackMontage;
};