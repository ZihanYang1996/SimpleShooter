// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CustomMoveTo.h"

#include "AIController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_CustomMoveTo::UBTTask_CustomMoveTo()
{
	NodeName = TEXT("Custom Move To");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CustomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
			// Check if the selected key is an actor or a vector
			bool bIsActor = BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass();
			AActor* Player{};
			FVector PlayerLocation;
			float DistanceToPlayer;
			if (bIsActor)
			{
				Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
				PlayerLocation = Player->GetActorLocation();
				DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerLocation);
			}
			else
			{
				PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
				DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerLocation);
			}
			
			if (DistanceToPlayer > MyMemory->AcceptanceRadius)
			{
				if (bIsActor)
				{
					AIController->MoveToActor(Player);
					AIController->SetFocus(Player);
				}
				else
				{
					AIController->MoveToLocation(PlayerLocation);
					AIController->SetFocalPoint(PlayerLocation);
				}
				
				return EBTNodeResult::InProgress;
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_CustomMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
			bool bIsActor = BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass();
			AActor* Player{};
			FVector PlayerLocation;
			float DistanceToPlayer;
			if (bIsActor)
			{
				Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
				PlayerLocation = Player->GetActorLocation();
				DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerLocation);
			}
			else
			{
				PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
				DistanceToPlayer = FVector::Dist(AICharacter->GetActorLocation(), PlayerLocation);
			}
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Distance to player: %f"), DistanceToPlayer));
			if (DistanceToPlayer <= MyMemory->AcceptanceRadius)
			{
				AIController->StopMovement();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				if (bIsActor)
				{
					AIController->MoveToActor(Player);
					AIController->SetFocus(Player);
				}
				else
				{
					AIController->MoveToLocation(PlayerLocation);
					AIController->SetFocalPoint(PlayerLocation);
				}
			}
		}
	}
}

uint16 UBTTask_CustomMoveTo::GetInstanceMemorySize() const
{
	return sizeof(BT_MoveToStopShooting);
}
