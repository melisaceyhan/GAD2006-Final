// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IsPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class LABWORK4_API UBTS_IsPlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	//Constructor
	UBTS_IsPlayerInRange();

	//Method for when service is called
	void OnBecomeRelevant(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem);

	//Field for range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float m_fRange = 100.0f;
	
};
