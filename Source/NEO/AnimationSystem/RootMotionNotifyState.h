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

    // 通知開始時の処理
    void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

    // 通知中常に処理
    void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);

    // 通知終了時の処理
    void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }

    // 疑似的にルートモーションを再現
    void RootMotion(USkeletalMeshComponent* MeshComp, float FrameDeltaTime);

private:

    // FPS取得
    float GetFps();

    // オーナーのアクターを取得
    AActor* GetOwnerActor(USkeletalMeshComponent* MeshComp);

    
private:
    UPROPERTY(EditAnywhere,Category = "Status",meta = (AllowPrivateAcces = "true"))
        float MoveSpeed;
};
