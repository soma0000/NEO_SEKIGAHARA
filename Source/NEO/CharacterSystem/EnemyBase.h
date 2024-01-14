// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API AEnemyBase : public ACharacterBase
{

	GENERATED_BODY()
public:
	AEnemyBase();
	
protected:

	UPROPERTY()
	bool IsAreaEnemy;


public:

	//この敵がバトルエリアの敵かどうかを設定するための関数
	UFUNCTION()
	void SetIsAreaEnemy(bool _IsAreaEnemy);

	//この敵がバトルエリアの敵かどうかを取得するための関数
	UFUNCTION()
	const bool GetIsAreaEnemy();
		

};
