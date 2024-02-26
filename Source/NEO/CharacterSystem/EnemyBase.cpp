// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "NEO/WeaponSystem/WeaponComponent.h"



AEnemyBase::AEnemyBase()
	: IsAreaEnemy(true)
{
}

//���̓G���o�g���G���A�̓G���ǂ�����ݒ肷�邽�߂̊֐�

void AEnemyBase::SetIsAreaEnemy(bool _IsAreaEnemy)
{
	//���̓G���o�g���G���A�̓G���ǂ����������Ƃ��Ď擾����
	IsAreaEnemy = _IsAreaEnemy;
}


//���̓G���o�g���G���A�̓G���ǂ������擾���邽�߂̊֐�

const bool AEnemyBase::GetIsAreaEnemy()
{
	return IsAreaEnemy;
}
