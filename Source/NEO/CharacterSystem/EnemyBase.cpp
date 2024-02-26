// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "NEO/WeaponSystem/WeaponComponent.h"



AEnemyBase::AEnemyBase()
	: IsAreaEnemy(true)
{
}

//この敵がバトルエリアの敵かどうかを設定するための関数

void AEnemyBase::SetIsAreaEnemy(bool _IsAreaEnemy)
{
	//この敵がバトルエリアの敵かどうかを引数として取得する
	IsAreaEnemy = _IsAreaEnemy;
}


//この敵がバトルエリアの敵かどうかを取得するための関数

const bool AEnemyBase::GetIsAreaEnemy()
{
	return IsAreaEnemy;
}
