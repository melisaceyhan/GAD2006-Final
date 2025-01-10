#include "BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIController.h"
#include "CombatComponent.h"  
#include "Kismet/GameplayStatics.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit)
{
    NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
    // Get Nav System
    UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    if (pNavSystem)
    {
        // Get AI controller from behavior tree component
        AAIController* pAIController = a_pTreeComp.GetAIOwner();

        // Get blackboard from AI controller
        UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(
            pAIController->GetComponentByClass(UBlackboardComponent::StaticClass())
        );

        if (!pBlackboard)
        {
            return EBTNodeResult::Failed; // Return failure if blackboard is not found
        }

        // Get the CombatComponent attached to the AI's pawn
        UCombatComponent* pCombatComp = pAIController->GetPawn()->FindComponentByClass<UCombatComponent>();

        // Check if CombatComponent exists and the AI is not dying, staggered, or attacking
        if (pCombatComp && !pCombatComp->IsDying() && !pCombatComp->IsStaggered() && !pCombatComp->IsAttacking())
        {
            // Get the player location from the blackboard
            FVector pLocation = pBlackboard->GetValueAsVector(GetSelectedBlackboardKey());

            // Command AI to move to player's location
            pAIController->MoveToLocation(pLocation);
        }
        else
        {
            // If conditions are not met, return failure
            return EBTNodeResult::Failed;
        }
    }

    // If everything is good, finish the task with success
    FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

    return EBTNodeResult::Succeeded;
}
