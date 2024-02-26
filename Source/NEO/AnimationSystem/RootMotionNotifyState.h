// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RootMotionNotifyState.generated.h"

UCLASS()
class NEO_API URootMotionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

    // �ʒm�J�n���̏���
    void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

    // �ʒm����ɏ���
    void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);

    // �ʒm�I�����̏���
    void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }

    // �^���I�Ƀ��[�g���[�V�������Č�
    void RootMotion(USkeletalMeshComponent* MeshComp, float FrameDeltaTime);

private:

    // FPS�擾
    float GetFps();

    // �I�[�i�[�̃A�N�^�[���擾
    AActor* GetOwnerActor(USkeletalMeshComponent* MeshComp);

    
private:
    UPROPERTY(EditAnywhere,Category = "Status",meta = (AllowPrivateAcces = "true"))
        float MoveSpeed;
};
