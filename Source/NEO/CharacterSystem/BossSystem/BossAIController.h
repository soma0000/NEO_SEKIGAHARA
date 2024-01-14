// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API ABossAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
