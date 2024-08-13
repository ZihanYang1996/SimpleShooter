// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CustomMoveTo.generated.h"

class AShooterCharacter;
/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_CustomMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_CustomMoveTo();

private:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 500.f;

	struct BT_MoveToStopShooting
	{
		float AcceptanceRadius;
	};

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};
