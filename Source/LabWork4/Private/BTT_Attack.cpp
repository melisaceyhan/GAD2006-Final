#pragma once
#include "BTT_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "CombatComponent.h"

UBTT_Attack::UBTT_Attack(FObjectInitializer const& a_pObjectInit)
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
    // Get the combat component
    UCombatComponent* pCombatComp = Cast<UCombatComponent>(a_pTreeComp.GetAIOwner()->GetPawn()->GetComponentByClass(UCombatComponent::StaticClass()));

    // Return early if this controller doesn�t have a combat component
    if (!pCombatComp)
    {
        return EBTNodeResult::Failed;
    }

    // Check if any previous attacks are finished
    if (!pCombatComp->IsAttacking())
    {
        // Get blackboard from AI controller
        UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(a_pTreeComp.GetAIOwner()->GetComponentByClass(UBlackboardComponent::StaticClass()));

        // Get bool value from key to check whether we can attack
        bool bCanAttack = pBlackboard->GetValueAsBool(GetSelectedBlackboardKey());
        if (bCanAttack)
        {
            // Run Boss KickAttack method
            pCombatComp->Attack(Attack);
        }
    }
    FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);
    // Finish execution
    return EBTNodeResult::Succeeded;
}
