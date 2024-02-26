// Fill out your copyright notice in the Description page of Project Settings.


#include "RootMotionNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"
#include "NEO/CharacterSystem/CharacterBase.h"


/*
 * �֐����@�@�@�@�FNotifyBegin()
 * �������e�@�@�@�F�ʒm�J�n���ɍs������
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �����R�@�@�@�@�Ffloat TotalDuration�E�E�E�A�j���[�V�����Đ������v����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void URootMotionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
}


/*
 * �֐����@�@�@�@�FNotifyTick()
 * �������e�@�@�@�F�ʒm����ɍs������
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ����̃A�j���[�V����
 * �����R�@�@�@�@�Ffloat FrameDeltaTime�E�E�E1�t���[���̎���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void URootMotionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	RootMotion(MeshComp,FrameDeltaTime);
}


/*
 * �֐����@�@�@�@�FNotifyEnd()
 * �������e�@�@�@�F�ʒm�I�����ɍs������
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ����Ă����A�j���[�V����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void URootMotionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}


/*
 * �֐����@�@�@�@�FRootMotion()
 * �������e�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�X�P���^�����b�V��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void URootMotionNotifyState::RootMotion(USkeletalMeshComponent* MeshComp, float FrameDeltaTime)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }

	// ���݌����Ă�������Ɉړ�����
	FVector ForwardVector = OwnerActor->GetActorForwardVector();
	FVector newVector = ForwardVector * MoveSpeed * FrameDeltaTime;

	// �L�����N�^�[�x�[�X�ɃL���X�g���ĕǂ����邩�`�F�b�N
	ACharacterBase* pOwner = Cast<ACharacterBase>(OwnerActor);

	// �ǂ��������烊�^�[��
	if (pOwner->WallCheck(MoveSpeed * FrameDeltaTime)) { return; }

	// ���Z
	OwnerActor->AddActorWorldOffset(newVector, false);
}


/*
 * �֐����@�@�@�@�FGetOwnerActor()
 * �������e�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �߂�l�@�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 */
AActor* URootMotionNotifyState::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// �v���C���[�̃x�[�X�N���X�擾
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}