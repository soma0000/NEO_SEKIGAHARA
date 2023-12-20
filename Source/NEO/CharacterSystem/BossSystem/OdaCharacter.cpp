// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaCharacter.h"

/*
 * �֐����@�@�@�@�FOdaUlt()
 * �������e�@�@�@�F�K�E�Z�U�������邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaCharacter::OdaUlt(int Timer)
{

	if (Timer % 30 == 0 && isUltShotTiming != true)
	{
		isUltShotTiming = true;
	}
	//�������킹�ɍs��
	if (isUltShotTiming != true)
	{
		if (GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
		{
			BossMoveY(OdaSpeed.Flont);
		}
		else
		{
			BossMoveY(-OdaSpeed.Flont);
		}
	}

	//���[�V����(�A�j���[�V����)���N��������
	else if (isMotionPlaying.isUltPlay == true)
	{
		//�A�j���[�V�����𗬂�
		AnimationPlay(AnimMontage_BossUltimate,&isMotionPlaying.isUltPlay);
		//�{�X���m�b�N�o�b�N���Ȃ��悤�ɂ���
		BossHPRock();
	}

	//�Ռ��g���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isShockWaveSpawnTiming == true)
	{
		//�_���[�W�l����
		BladeDamage = UltShockWaveDamage;
		//�A�N�^�[�̃X�|�[������(ShockWaveSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isShockWaveSpawnTiming = false;
	}

	//�A�j���[�V�����̍Ō�ɕK�E�Z���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isUltSpawnTiming == true)
	{
		//�A�N�^�[�̃X�|�[������(UltSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(UltSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isUltSpawnTiming = false;
	}



	//250�t���[����������
	if (Timer > iAnimTime.Ultimate)
	{
		//�X�e�[�g��؂�ւ���
		OdaMoveEnum = ECPPOdaEnum::Moveflont;
		//�؂�ւ���ɂ������ĕϐ�������������
		TimeCounter = 0;
		isUltShotTiming = false;
	}
}
