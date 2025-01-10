#pragma once
#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

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
bool UCombatComponent::IsStaggered()
{
    // Get owner of component and then get their animator
    UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

    // Return early if the anim instance wasn't found
    if (!pAnimInst)
        return false;

    // Check if any stagger animations are playing
    return pAnimInst->Montage_IsPlaying(m_pHurtMontage);
}

bool UCombatComponent::IsDying()
{
    // Get owner of component and then get their animator
    UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

    // Return early if the anim instance wasn't found
    if (!pAnimInst)
        return false;

    // Check if any dying animations are playing
    return pAnimInst->Montage_IsPlaying(m_pDeathMontage);
}
void UCombatComponent::GenerateHitSphere(FVector a_vLocation, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug)
{
    // Init hit results
    TArray<FHitResult> outHits;
    TArray<AActor*> ignoredActors;
    ignoredActors.Add(GetOwner());

    // Use Sphere Shape
    FCollisionShape sphereShape;
    sphereShape.ShapeType = ECollisionShape::Sphere;
    sphereShape.SetSphere(a_fRadius);

    // Set the query params
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(GetOwner());

    // Sweep trace at typical contact point of swing
    GetWorld()->SweepMultiByChannel(outHits, a_vLocation, a_vLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);

    // Debug Trace to show visual representation
    if (a_bDebug)
    {
        // Sweep Trace at typical contact point of swing with debugging visuals
        UKismetSystemLibrary::SphereTraceMulti(GetWorld(), a_vLocation, a_vLocation,
            a_fRadius,
            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), true, ignoredActors,
            EDrawDebugTrace::ForDuration, outHits, true, FColor::Red);
    }

    // Check that a collision happened
    if (outHits.Num() > 0)
    {
        // Iterate through the hit results
        for (auto i = outHits.CreateIterator(); i; i++)
        {
            // Check if actor is part of damaged actor set
            if (m_sDamagedActors.Contains(i->GetActor()))
                continue;
            else
                // Add to set if not
                m_sDamagedActors.Add(i->GetActor());

            // Try casting actor to a character
            ACharacter* pVictim = Cast<ACharacter>(i->GetActor());

            // Handle damage
            if (pVictim)
                HandleDamage(pVictim, a_vLocation, a_sAttackStats);
        }
    }
}

void UCombatComponent::GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug)
{
    // Init hit results
    TArray<FHitResult> outHits;
    TArray<AActor*> ignoredActors;
    ignoredActors.Add(GetOwner());

    // Use Sphere Shape
    FCollisionShape sphereShape;
    sphereShape.ShapeType = ECollisionShape::Sphere;
    sphereShape.SetSphere(a_fRadius);

    // Set the query params
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(GetOwner());

    // Sweep trace at typical contact point of swing
    GetWorld()->SweepMultiByChannel(outHits, a_vBeginLoc, a_vEndLoc , FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);

    // Debug Trace to show visual representation
    if (a_bDebug)
    {
        // Sweep Trace at typical contact point of swing with debugging visuals
        UKismetSystemLibrary::SphereTraceMulti(GetWorld(), a_vBeginLoc, a_vEndLoc,
            a_fRadius,
            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), true, ignoredActors,
            EDrawDebugTrace::ForDuration, outHits, true, FColor::Red);
    }

    // Check that a collision happened
    if (outHits.Num() > 0)
    {
        // Iterate through the hit results
        for (auto i = outHits.CreateIterator(); i; i++)
        {
            FVector midpoint = (a_vBeginLoc + a_vEndLoc) / 2;
            // Check if actor is part of damaged actor set
            if (m_sDamagedActors.Contains(i->GetActor()))
                continue;
            else
                // Add to set if not
                m_sDamagedActors.Add(i->GetActor());

            // Try casting actor to a character
            ACharacter* pVictim = Cast<ACharacter>(i->GetActor());

            // Handle damage
            if (pVictim)
                HandleDamage(pVictim, midpoint, a_sAttackStats);
        }
    }
}

// Combat Component Implementation
void UCombatComponent::HandleDamage(ACharacter* a_pVictim, FVector a_vLoc, FAttackStats a_sAttackStats)
{
    // Get the character's combat component
    UCombatComponent* pVicCombComp = Cast<UCombatComponent>(a_pVictim->GetComponentByClass(UCombatComponent::StaticClass()));

    // Return if victim doesn't have a combat component
    if (!pVicCombComp)
        return;

    // Return if player is dying or dead
    if (pVicCombComp->m_fHealth <= 0)
        return;

    // Get owner of component and then get their animator
    UAnimInstance* pAnimInst = Cast<ACharacter>(pVicCombComp->GetOwner())->GetMesh()->GetAnimInstance();

    // Return early if the anim instance wasn't found
    if (!pAnimInst)
        return;

    // Play the death montage and death sound
    if (pVicCombComp->m_pHurtMontage)
        pAnimInst->Montage_Play(pVicCombComp->m_pHurtMontage);

    // Deal damage
    pVicCombComp->TakeDamage(a_sAttackStats.sDamage);
}

void UCombatComponent::TakeDamage(float a_fDamage)
{
    // Check if already dying
    if (IsDying())
        return;

    // Negate health by damage amount
    m_fHealth -= a_fDamage;

    // Handle death
    if (m_fHealth <= 0.0f)
        Die();
}
void UCombatComponent::Die()
{
    // Get owner of component and then get their animator
    UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

    // Return early if the anim instance wasn't found
    if (!pAnimInst)
        return;

    // Play the death montage and death sound
    if (m_pDeathMontage)
        pAnimInst->Montage_Play(m_pDeathMontage);

    // Disable character movement
    Cast<ACharacter>(GetOwner())->GetCharacterMovement()->DisableMovement();
}