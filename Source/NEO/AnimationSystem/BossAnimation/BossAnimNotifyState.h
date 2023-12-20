// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/AnimationSystem/CharacterAnimNotifyState.h"
#include "BossAnimNotifyState.generated.h"

UENUM(BlueprintType)
enum class EBossNotifyStateType :uint8
{
    NotifyType_RootMotion_BossAttack1   UMETA(DisplayName = "RootMotion_BossAttack1"),
    NotifyType_BossAttack1              UMETA(DisplayName = "BossAttack1"),
};

UCLASS()
class NEO_API UBossAnimNotifyState : public UCharacterAnimNotifyState
{
    GENERATED_BODY()

     virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }

public:
     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
     EBossNotifyStateType NotifyType;

    // �ʒm�J�n���ɍs������
     virtual void NotifyBeginAction(AActor* _Owner, UAnimSequenceBase* Animation, float TotalDuration) { return; }

    // �ʒm����ɂɍs������
     virtual void NotifyTickAction(AActor* _Owner, UAnimSequenceBase* Animation, float FrameDeltaTime);

    // �ʒm�I�����ɍs������
    virtual void NotifyEndAction(AActor* _Owner, UAnimSequenceBase* Animation) { return; }

};
