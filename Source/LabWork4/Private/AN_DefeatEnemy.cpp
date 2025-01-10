// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DefeatEnemy.h"
#include "GameFramework/Actor.h" 


void UAN_DefeatEnemy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* pOwner = MeshComp->GetOwner();

	pOwner->K2_DestroyActor();
}