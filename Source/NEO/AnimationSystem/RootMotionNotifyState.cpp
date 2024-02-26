// Fill out your copyright notice in the Description page of Project Settings.


#include "RootMotionNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"
#include "NEO/CharacterSystem/CharacterBase.h"


/*
 * 関数名　　　　：NotifyBegin()
 * 処理内容　　　：通知開始時に行う処理
 * 引数１　　　　：USkeletalMeshComponent* MeshComp・・・オーナーのメッシュ
 * 引数２　　　　：UAnimSequenceBase* Animation・・・再生するアニメーション
 * 引数３　　　　：float TotalDuration・・・アニメーション再生総所要時間
 * 戻り値　　　　：なし
 */
void URootMotionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
}


/*
 * 関数名　　　　：NotifyTick()
 * 処理内容　　　：通知中常に行う処理
 * 引数１　　　　：USkeletalMeshComponent* MeshComp・・・オーナーのメッシュ
 * 引数２　　　　：UAnimSequenceBase* Animation・・・再生中のアニメーション
 * 引数３　　　　：float FrameDeltaTime・・・1フレームの時間
 * 戻り値　　　　：なし
 */
void URootMotionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	RootMotion(MeshComp,FrameDeltaTime);
}


/*
 * 関数名　　　　：NotifyEnd()
 * 処理内容　　　：通知終了時に行う処理
 * 引数１　　　　：USkeletalMeshComponent* MeshComp・・・オーナーのメッシュ
 * 引数２　　　　：UAnimSequenceBase* Animation・・・再生していたアニメーション
 * 戻り値　　　　：なし
 */
void URootMotionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}


/*
 * 関数名　　　　：RootMotion()
 * 処理内容　　　：オーナーをAActor*で取得
 * 引数１　　　　：USkeletalMeshComponent* MeshComp・・・スケルタルメッシュ
 * 戻り値　　　　：なし
 */
void URootMotionNotifyState::RootMotion(USkeletalMeshComponent* MeshComp, float FrameDeltaTime)
{
	// オーナー取得
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }

	// 現在向いている方向に移動する
	FVector ForwardVector = OwnerActor->GetActorForwardVector();
	FVector newVector = ForwardVector * MoveSpeed * FrameDeltaTime;

	// キャラクターベースにキャストして壁があるかチェック
	ACharacterBase* pOwner = Cast<ACharacterBase>(OwnerActor);

	// 壁があったらリターン
	if (pOwner->WallCheck(MoveSpeed * FrameDeltaTime)) { return; }

	// 加算
	OwnerActor->AddActorWorldOffset(newVector, false);
}


/*
 * 関数名　　　　：GetOwnerActor()
 * 処理内容　　　：オーナーをAActor*で取得
 * 引数１　　　　：USkeletalMeshComponent* MeshComp・・・オーナーのメッシュ
 * 戻り値　　　　：オーナーをAActor*で取得
 */
AActor* URootMotionNotifyState::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// プレイヤーのベースクラス取得
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}