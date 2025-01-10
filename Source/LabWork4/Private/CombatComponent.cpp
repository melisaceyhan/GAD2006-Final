#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

// Constructor implementation
UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// BeginPlay implementation
void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

// TickComponent implementation
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Attack method
void UCombatComponent::Attack(FName a_nAttackName)
{
    // Check if map has elements
    if (m_AttackMap.Num() == 0)
        return;

    // Get owner of component and then get their animator
    UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

    // Return early if the anim instance wasn't found
    if (!pAnimInst)
        return;

    // Get the attack from the map
    FAttack* pAttack = m_AttackMap.Find(a_nAttackName);

    // Return early if attack not found
    if (!pAttack)
        return;

    // Get the attack montage from attack
    UAnimMontage* pAttackMontage = pAttack->pAttackMontage;

    // Play the attack if valid
    if (pAttackMontage)
        pAnimInst->Montage_Play(pAttackMontage);
}

// StopAttack method
void UCombatComponent::StopAttack(float a_fBlendOutTime, FName aAttackName)
{
    // Check if map has elements
    if (m_AttackMap.Num() == 0)
        return;

    // Get owner of component and then get their animator
    UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

    // Return early if the anim instance wasn't found
    if (!pAnimInst)
        return;

    // Get the attack from the map
    FAttack* pAttack = m_AttackMap.Find(aAttackName);

    // Return early if attack not found
    if (!pAttack)
        return;

    // Get the attack montage from attack
    UAnimMontage* pAttackMontage = pAttack->pAttackMontage;

    // Stop the attack if valid
    if (pAttackMontage)
        pAnimInst->Montage_Stop(a_fBlendOutTime, pAttackMontage);
}

// IsAttacking method
bool UCombatComponent::IsAttacking()
{
    // Check if map has elements
    if (m_AttackMap.Num() == 0)
        return false;

    // Get owner of component and then get their animator
    UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

    // Return early if the anim instance wasn't found
    if (!pAnimInst)
        return false;

    // Get map keys
    TArray<FName> aKeys;
    m_AttackMap.GetKeys(aKeys);

    // Check all attacks to see if any of them are currently playing
    for (int i = 0; i < aKeys.Num(); i++)
    {
        // Return true when you find an attack playing
        if (pAnimInst->Montage_IsPlaying(m_AttackMap.Find(aKeys[i])->pAttackMontage))
            return true;
    }

    return false;
}
