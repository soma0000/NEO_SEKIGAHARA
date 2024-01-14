// Fill out your copyright notice in the Description page of Project Settings.


#include "NEO/CharacterSystem/BossSystem/BossAIController.h"
#include <Kismet/GameplayStatics.h>

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	

	// BlackboardにプレイヤーをTaragetActorとして登録
	auto pBlackBoard = GetBlackboardComponent();
	if (pBlackBoard)
	{
		pBlackBoard->SetValueAsObject(TEXT("PlayerObject"), UGameplayStatics::GetPlayerPawn(this, 0));

	}
}