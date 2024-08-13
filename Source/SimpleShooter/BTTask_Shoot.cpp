// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AShooterCharacter* AIControlledCharacter = Cast<AShooterCharacter>(AIController->GetPawn()))
		{
			if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsShooting")))
			{
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Shoot!"));
				if (AActor* PlayerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player"))))
				{
					AIController->SetFocus(PlayerActor);
				}
				AIControlledCharacter->PullTrigger();
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsShooting"), true);
			}
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_Shoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AShooterCharacter* AIControlledCharacter = Cast<AShooterCharacter>(AIController->GetPawn()))
		{
			if (AActor* PlayerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player"))))
			{
				AIController->SetFocus(PlayerActor);
				float Distance = FVector::Dist(AIControlledCharacter->GetActorLocation(), PlayerActor->GetActorLocation());
				if (Distance > AcceptanceRadius)
				{
					AIControlledCharacter->ReleaseTrigger();
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsShooting"), false);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}
		}
	}
}
