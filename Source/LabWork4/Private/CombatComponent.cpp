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
void UCombatComponent::Attack(FName a_AttackName)
{
    if (m_AttackMap.Num() == 0)
        return;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
        return;

    UAnimInstance* pAnimInst = OwnerCharacter->GetMesh()->GetAnimInstance();
    if (!pAnimInst)
        return;

    const FAttack* pAttack = m_AttackMap.Find(a_AttackName);
    if (!pAttack || !pAttack->pAttackMontage)
        return;

    pAnimInst->Montage_Play(pAttack->pAttackMontage);
}

// StopAttack method
void UCombatComponent::StopAttack(float a_fBlendOutTime, FName a_AttackName)
{
    if (m_AttackMap.Num() == 0)
        return;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
        return;

    UAnimInstance* pAnimInst = OwnerCharacter->GetMesh()->GetAnimInstance();
    if (!pAnimInst)
        return;

    const FAttack* pAttack = m_AttackMap.Find(a_AttackName);
    if (!pAttack || !pAttack->pAttackMontage)
        return;

    pAnimInst->Montage_Stop(a_fBlendOutTime, pAttack->pAttackMontage);
}

// IsAttacking method
bool UCombatComponent::IsAttacking() const
{
    if (m_AttackMap.Num() == 0)
        return false;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
        return false;

    UAnimInstance* pAnimInst = OwnerCharacter->GetMesh()->GetAnimInstance();
    if (!pAnimInst)
        return false;

    TArray<FName> aKeys;
    m_AttackMap.GetKeys(aKeys);

    for (const FName& AttackName : aKeys)
    {
        const FAttack* pAttack = m_AttackMap.Find(AttackName);
        if (pAttack && pAttack->pAttackMontage && pAnimInst->Montage_IsPlaying(pAttack->pAttackMontage))
        {
            return true;
        }
    }

    return false;
}
