// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToStopShooting.h"

#include "AIController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToStopShooting::UBTTask_MoveToStopShooting()
{
	NodeName = TEXT("Move To Stop Shooting");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToStopShooting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BT_MoveToStopShooting* MyMemory = (BT_MoveToStopShooting*)(NodeMemory);
	MyMemory->AIController = OwnerComp.GetAIOwner();
	if (MyMemory->AIController)
	{
		MyMemory->AIControlledCharacter = Cast<AShooterCharacter>(MyMemory->AIController->GetPawn());
		if (MyMemory->AIControlledCharacter)
		{
			MyMemory->AcceptanceRadius = AcceptanceRadius;

			FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
			float DistanceToPlayer = FVector::Dist(MyMemory->AIControlledCharacter->GetActorLocation(), PlayerLocation);

			if (DistanceToPlayer > MyMemory->AcceptanceRadius)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Moving and stop shooting"));
				MyMemory->AIControlledCharacter->ReleaseTrigger();
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsShooting"), false);

				MyMemory->AIController->MoveToLocation(PlayerLocation);	
				return EBTNodeResult::InProgress;
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_MoveToStopShooting::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	BT_MoveToStopShooting* MyMemory = (BT_MoveToStopShooting*)(NodeMemory);
	if (!MyMemory->AIController || !MyMemory->AIControlledCharacter)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	float DistanceToPlayer = FVector::Dist(MyMemory->AIControlledCharacter->GetActorLocation(), PlayerLocation);

	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Distance to player: %f"), DistanceToPlayer));
	if (DistanceToPlayer <= MyMemory->AcceptanceRadius)
	{
		MyMemory->AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		MyMemory->AIController->MoveToLocation(PlayerLocation, 0);
	}
}
