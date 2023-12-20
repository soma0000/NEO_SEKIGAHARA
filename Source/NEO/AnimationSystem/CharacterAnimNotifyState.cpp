// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/CharacterSystem/CharacterBase.h"


/*
 * �֐����@�@�@�@�FNotifyBegin()
 * �������e�@�@�@�F�ʒm�J�n���ɍs������
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �����R�@�@�@�@�Ffloat TotalDuration�E�E�E�A�j���[�V�����Đ������v����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UCharacterAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// �ʒm�J�n���̏���
	NotifyBeginAction(OwnerActor, Animation,TotalDuration);
}


/*
 * �֐����@�@�@�@�FNotifyTick()
 * �������e�@�@�@�F�ʒm����ɍs������
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ����̃A�j���[�V����
 * �����R�@�@�@�@�Ffloat FrameDeltaTime�E�E�E1�t���[���̎���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UCharacterAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// �ʒm����ɏ���
	NotifyTickAction(OwnerActor,Animation,FrameDeltaTime);
}


/*
 * �֐����@�@�@�@�FNotifyEnd()
 * �������e�@�@�@�F�ʒm�I�����ɍs������
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ����Ă����A�j���[�V����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UCharacterAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// �ʒm�I�����̏���
	NotifyEndAction(OwnerActor,Animation);
}


/*
 * �֐����@�@�@�@�FGetOwnerActor()
 * �������e�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 * �����P�@�@�@�@�Fbool _isLookRight�E�E�E�����Ă������
 * �����Q�@�@�@�@�Ffloat _moveValue�E�E�E���t���[���ړ��l
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UCharacterAnimNotifyState::RootMotion(AActor* _pOwner,float _moveValue)
{
	if (!_pOwner) { return; }

	// �O���x�N�g���擾
	FVector ForwardVector = _pOwner->GetActorForwardVector();

	// ���݌����Ă�������Ɉړ�����
	FVector newVector = ForwardVector * _moveValue;

	// �L�����N�^�[�x�[�X�ɃL���X�g���ĕǂ����邩�`�F�b�N
	ACharacterBase* pOwner = Cast<ACharacterBase>(_pOwner);

	// �ǂ��������烊�^�[��
	if (pOwner->WallCheck(_moveValue)) { return; }

	// ���Z
	_pOwner->AddActorWorldOffset(newVector, false);
}



/*
 * �֐����@�@�@�@�FGetOwnerActor()
 * �������e�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �߂�l�@�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 */
AActor* UCharacterAnimNotifyState::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// �v���C���[�̃x�[�X�N���X�擾
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}