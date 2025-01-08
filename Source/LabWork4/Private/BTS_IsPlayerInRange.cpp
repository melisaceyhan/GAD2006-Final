#include "BTS_IsPlayerInRange.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_IsPlayerInRange::UBTS_IsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Range");
}

void UBTS_IsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeName)
{
	// Super Call
	Super::OnBecomeRelevant(a_pTreeComp, a_pNodeName);

	// Get blackboard from AI controller
	UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(a_pTreeComp.GetAIOwner()->GetComponentByClass(UBlackboardComponent::StaticClass()));

	// Set blackboard key to whether or not player is in range
	pBlackboard->SetValueAsBool(
		GetSelectedBlackboardKey(),
		a_pTreeComp.GetAIOwner()->GetPawn()->GetDistanceTo(
			Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		) <= m_fRange
	);
}
