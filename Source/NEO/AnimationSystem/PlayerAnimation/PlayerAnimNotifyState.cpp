
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/CharacterSystem/PlayerSystem/PlayerBase.h"


/*
 * 関数名　　　　：NotifyBeginAction()
 * 処理内容　　　：通知開始時に行う処理
 * 引数１　　　　：AActor* _Owner・・・オーナー
 * 引数２　　　　：UAnimSequenceBase* Animation・・・再生するアニメーション
 * 引数３　　　　：float TotalDuration・・・再生にかかる時間
 * 戻り値　　　　：なし
 */
void UPlayerAnimNotifyState::NotifyBeginAction(AActor* _Owner, UAnimSequenceBase* Animation, float TotalDuration)
{

}



/*
 * 関数名　　　　：NotifyAction()
 * 処理内容　　　：通知時に行う処理
 * 引数１　　　　：AActor* _Owner・・・オーナー
 * 引数２　　　　：UAnimSequenceBase* Animation・・・再生するアニメーション
 * 戻り値　　　　：なし
 */
void UPlayerAnimNotifyState::NotifyTickAction(AActor* _Owner, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    // アニメーションでの移動開始
}


/*
 * 関数名　　　　：NotifyEndAction()
 * 処理内容　　　：通知終了時に行う処理
 * 引数１　　　　：AActor* _Owner・・・オーナー
 * 引数２　　　　：UAnimSequenceBase* Animation・・・再生するアニメーション
 * 戻り値　　　　：なし
 */
void UPlayerAnimNotifyState::NotifyEndAction(AActor* _Owner, UAnimSequenceBase* Animation)
{

}


