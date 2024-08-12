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
	BT_MoveToStopShooting* MyMemory = CastInstanceNodeMemory<BT_MoveToStopShooting>(NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		AShooterCharacter* AICharacter = Cast<AShooterCharacter>(AIController->GetPawn());
		if (AICharacter)
		{
			MyMemory->AcceptanceRadius = AcceptanceRadius;

			FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
			float DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerLocation);

			if (DistanceToPlayer > MyMemory->AcceptanceRadius)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Moving and stop shooting"));
				AICharacter->ReleaseTrigger();
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsShooting"), false);

				AIController->MoveToLocation(PlayerLocation);
				return EBTNodeResult::InProgress;
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_MoveToStopShooting::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	BT_MoveToStopShooting* MyMemory = CastInstanceNodeMemory<BT_MoveToStopShooting>(NodeMemory);
	if (MyMemory == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController != nullptr)
	{
		AShooterCharacter* AICharacter = Cast<AShooterCharacter>(AIController->GetPawn());
		if (AICharacter != nullptr)
		{
			FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
			// Print AcceptanceRadius
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Is this node an instance? %s"), bCreateNodeInstance ? TEXT("Yes") : TEXT("No")));
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Acceptance Radius (Memory): %f"), MyMemory->AcceptanceRadius++));
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Acceptance Radius: %f"), AcceptanceRadius++));
			FVector MyLocation = AICharacter->GetActorLocation();
			float DistanceToPlayer = FVector::Dist(MyLocation, PlayerLocation);


			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Distance to player: %f"), DistanceToPlayer));
			if (DistanceToPlayer <= MyMemory->AcceptanceRadius)
			{
				AIController->StopMovement();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				AIController->MoveToLocation(PlayerLocation, 0);
			}
		}
	}
}

uint16 UBTTask_MoveToStopShooting::GetInstanceMemorySize() const
{
	return sizeof(BT_MoveToStopShooting);
}
