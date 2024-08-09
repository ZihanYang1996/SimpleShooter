// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (AShooterCharacter* AIControlledCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsShooting")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shoot!"));
			AIControlledCharacter->PullTrigger();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsShooting"), true);
		}
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}