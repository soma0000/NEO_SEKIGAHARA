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

	//���̓G���o�g���G���A�̓G���ǂ�����ݒ肷�邽�߂̊֐�
	UFUNCTION()
	void SetIsAreaEnemy(bool _IsAreaEnemy);

	//���̓G���o�g���G���A�̓G���ǂ������擾���邽�߂̊֐�
	UFUNCTION()
	const bool GetIsAreaEnemy();
		

};
