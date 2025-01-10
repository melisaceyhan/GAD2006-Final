// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DefeatPlayer.h"
#include "GameFramework/Actor.h" 
#include "Kismet/GameplayStatics.h"

void UAN_DefeatPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UGameplayStatics::OpenLevel(MeshComp->GetOwner()->GetWorld(), FName(MeshComp->GetOwner()->GetWorld()->GetName()), true);
}