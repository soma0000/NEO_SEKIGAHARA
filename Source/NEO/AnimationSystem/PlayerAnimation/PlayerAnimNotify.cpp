// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/CharacterSystem/PlayerSystem/PlayerBase.h"


/*
 * �֐����@�@�@�@�FCanBePlaced()
 * �������e�@�@�@�F�G�f�B�^��Ɍ��J���邩�ǂ���
 * �����P�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �߂�l�@�@�@�@�Ftrue�Efalse�Ō��J����
 */
bool UPlayerAnimNotify::CanBePlaced(UAnimSequenceBase* Animation) const
{
    // �����^�[�W���݂̂Ɍ��J
    if (Animation->IsA(UAnimMontage::StaticClass()))
    {
        return true;
    }


    return false;
}


/*
 * �֐����@�@�@�@�FNotifyAction()
 * �������e�@�@�@�F�ʒm���ɍs������
 * �����P�@�@�@�@�FAActor* _Owner�E�E�E�I�[�i�[
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UPlayerAnimNotify::NotifyAction(AActor* _Owner, UAnimSequenceBase* Animation)
{
    // �v���C���[�̃x�[�X�N���X�擾
    APlayerBase* pPlayer = Cast<APlayerBase>(_Owner);

    if (pPlayer)
    {
        switch (NotifyType)
        {
        case EPlayerNotifyType::NotifyType_SetCollision:    // �����蔻��J�n�̏�����������
            pPlayer->SetCollision();
            break;
        case EPlayerNotifyType::NotifyType_SaveCombo:       // �R���{�p���̒ʒm��������
            pPlayer->ContinuationCombo();
            break;
        case EPlayerNotifyType::NotifyType_ResetCombo:      // �R���{���Z�b�g�̒ʒm��������
            pPlayer->ResetCombo();
            break;
        case EPlayerNotifyType::NotifyType_CanControl:      // �R���g���[���\�ɂȂ�ʒm��������
            pPlayer->ResetCombo();
            break;
        case EPlayerNotifyType::NotifyType_SlowDown:        // �A�j���[�V������x������ʒm��������
            pPlayer->SlowDownDeathAnimationRate();
            break;
        case EPlayerNotifyType::NotifyType_StopMontage:     // �A�j���[�V�����Đ����~�߂鏈��
            pPlayer->StopMontage();
            break;

        }
    }
}

