// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class AEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	//Constructor
	AEnemyController(FObjectInitializer const& a_pObjectInit);

	//Overrides
	void BeginPlay() override;
	void OnPossess(APawn* a_pPawn) override;

	//Method to get black board
	class UBlackboardComponent* GetBlackboard() const;

	//Tree components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTreeComponent* m_pBehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* m_pBehaviorTree;

private:
	class UBlackboardComponent* m_pBlackboard;

};
