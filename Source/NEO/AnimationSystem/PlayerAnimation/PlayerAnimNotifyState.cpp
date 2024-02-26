
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/CharacterSystem/PlayerSystem/PlayerBase.h"


/*
 * �֐����@�@�@�@�FNotifyBeginAction()
 * �������e�@�@�@�F�ʒm�J�n���ɍs������
 * �����P�@�@�@�@�FAActor* _Owner�E�E�E�I�[�i�[
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �����R�@�@�@�@�Ffloat TotalDuration�E�E�E�Đ��ɂ����鎞��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UPlayerAnimNotifyState::NotifyBeginAction(AActor* _Owner, UAnimSequenceBase* Animation, float TotalDuration)
{

}



/*
 * �֐����@�@�@�@�FNotifyAction()
 * �������e�@�@�@�F�ʒm���ɍs������
 * �����P�@�@�@�@�FAActor* _Owner�E�E�E�I�[�i�[
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UPlayerAnimNotifyState::NotifyTickAction(AActor* _Owner, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    // �A�j���[�V�����ł̈ړ��J�n
}


/*
 * �֐����@�@�@�@�FNotifyEndAction()
 * �������e�@�@�@�F�ʒm�I�����ɍs������
 * �����P�@�@�@�@�FAActor* _Owner�E�E�E�I�[�i�[
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UPlayerAnimNotifyState::NotifyEndAction(AActor* _Owner, UAnimSequenceBase* Animation)
{

}


