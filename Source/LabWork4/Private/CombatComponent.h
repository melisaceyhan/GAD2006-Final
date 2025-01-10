#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CombatStructs.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LABWORK4_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UCombatComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Map that holds attack information
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, FAttack> m_AttackMap;

    // Method for executing attacks based on name
    void Attack(FName a_AttackName);

    // Method for halting an attack early
    void StopAttack(float a_fBlendOutTime, FName a_AttackName);

    // Check if actor is attacking
    bool IsAttacking();

    //Health meter
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float m_fMaxHealth = 100.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float m_fHealth = m_fMaxHealth;

    //Stagger montages
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* m_pHurtMontage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* m_pDeathMontage;

    //Methods for checking if stagger or die
    bool IsStaggered();
    bool IsDying();

    //Damage handlers and trace generation handlers
    void GenerateHitSphere(FVector a_vLocation, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug = false);
    void GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug = false);
    void HandleDamage(ACharacter* a_pVictim, FVector a_vLoc, FAttackStats a_sAttackStats);

    //Set of damaged actors that is tracked during combat
    TSet<AActor*> m_sDamagedActors;

    //Methods for taking damage and death
    void TakeDamage(float a_fDamage);
    void Die();
};
