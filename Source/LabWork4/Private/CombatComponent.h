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
    bool IsAttacking() const;
};
