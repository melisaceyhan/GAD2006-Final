// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyController::AEnemyController(FObjectInitializer const& a_pObjectInit) 
{
	//Init  the tree and blackboard components
	m_pBehaviorTreeComponent = a_pObjectInit.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree Component"));
	m_pBlackboard = a_pObjectInit.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard Component"));
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	//Run the behavior tree
	RunBehaviorTree(m_pBehaviorTree);
	m_pBehaviorTreeComponent->StartTree(*m_pBehaviorTree);
}

void AEnemyController::OnPossess(APawn* a_pPawn)
{
	Super::OnPossess(a_pPawn);

	if (m_pBlackboard)
	{
		m_pBlackboard->InitializeBlackboard(*m_pBehaviorTree->BlackboardAsset);
	}
}
UBlackboardComponent* AEnemyController::GetBlackboard() const
{
	return m_pBlackboard;
}
