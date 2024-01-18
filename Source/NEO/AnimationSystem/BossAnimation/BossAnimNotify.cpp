// Fill out your copyright notice in the Description page of Project Settings.


#include "NEO/AnimationSystem/BossAnimation/BossAnimNotify.h"

#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

#include "NEO/CharacterSystem/BossSystem/OdaBase.h"

void UBossAnimNotify::Notify(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* Animation)
{
	// �擾
	TObjectPtr< AOdaBase> aOda = Cast< AOdaBase>(_MeshComp->GetOwner());
	//NULL�`�F�b�N
	if (aOda == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveOda is not Found"));
		return;
	}

	if (NotifyType == EBossNotifyType::NotifyType_BossAttack2)
	{
		// �Ռ��g������
		aOda->ShockWaveSpawnFlagChange();
	}
	else if (NotifyType == EBossNotifyType::NotifyType_BossAttack2Conbo)
	{
		aOda->Is2Combo();
	}
	else if (NotifyType == EBossNotifyType::NotifyType_BossAttacked)
	{
		aOda->toPlayerAttacked();
	}
	else if (NotifyType == EBossNotifyType::NotifyType_BossDeath)
	{
		aOda->WorldTimeReturn();
	}
	else if (NotifyType == EBossNotifyType::NotifyType_BossHPLock)
	{
		// �v���C���[��HP�����炳�Ȃ��悤�ɂ���
		aOda->HPLock();
	}
	else if (NotifyType == EBossNotifyType::NotifyType_BossUlt)
	{
		aOda->UltSpawnFlagChange(UltInfo);
	}
}
