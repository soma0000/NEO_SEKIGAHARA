// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionAssistComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "NiagaraFunctionLibrary.h"		
#include "PlayerSystem/NEOPlayerController.h"

#define DIRECTION_Y (90.f)

// Sets default values for this component's properties
UActionAssistComponent::UActionAssistComponent()
	: bUseCorrectAttackAngle(true)
	, bUseHitEffect(true)
	, bUseFaceCamera(true)
{
	// Tick()を毎フレーム呼ばないようにする
	PrimaryComponentTick.bCanEverTick = false;

	RayLength_CorrectAngle = 300.f;
	MaxCorrectAngle = 10.f;
}


// Called when the game starts
void UActionAssistComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UActionAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


/*
 * 関数名　　　　：CorrectAttackAngle()
 * 処理内容　　　：角度を補正して攻撃を当たりやすくする(直線状にいる敵)
 * 戻り値　　　　：なし
 */
void UActionAssistComponent::CorrectAttackAngle()
{
	// 機能のオン・オフ
	if (!bUseCorrectAttackAngle) { return; }

	// オーナー取得
	AActor* Owner = GetOwner();

	//　前方にいるエネミー取得
	AActor* InFrontOfEnemy = GetFrontActor();

	if (Owner && InFrontOfEnemy)
	{
		// プレイヤーと敵の方向を取得する
		FVector direction = InFrontOfEnemy->GetActorLocation() - Owner->GetActorLocation();
		direction.Normalize();

		// プレイヤーの向きを取得する
		FRotator CurrentRotation = Owner->GetActorRotation();

		// 補正角度取得
		float CorrectAngle = direction.Rotation().Yaw;

		// 最大角度を超えたら
		if (FMath::Abs(CorrectAngle) <= MaxCorrectAngle)
		{
			// 最大角度に補正
			CorrectAngle = (CorrectAngle < 0) ? (-MaxCorrectAngle) : (MaxCorrectAngle);
		}

		// 所有者の向きを変更する
		FRotator NewRotation(CurrentRotation.Pitch, CorrectAngle, CurrentRotation.Roll);
		Owner->SetActorRotation(NewRotation);
	}
}


/*
 * 関数名　　　　：SpawnEffect()
 * 引数１　　　　：UNiagaraSystem* _hitParticle・・・出現させるエフェクト
 * 引数２　　　　：FVector _spawnPos・・・・・・・・・出現させる場所
 * 処理内容　　　：エフェクトを出現する
 * 戻り値　　　　：なし
 */
void UActionAssistComponent::SpawnEffect(UNiagaraSystem* _hitParticle, FVector _spawnPos)
{
	// 機能のオン・オフ
	if (!bUseHitEffect) { return; }

	// エフェクトの情報が取得できなければリターン
	if (!_hitParticle) { return; }

	//エフェクトを出す
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner()->GetWorld(), _hitParticle, _spawnPos);
}

/*
 * 関数名　　　　：CameraShake()
 * 引数１　　　　：TSubclassOf<UCameraShakeBase> _shakePattern・・・揺らすパターン
 * 引数２　　　　：float _scale ・・・・・・・・・・・・・・・・・・強さ
 * 処理内容　　　：カメラシェイク
 * 戻り値　　　　：なし
 */
void UActionAssistComponent::CameraShake(TSubclassOf<UCameraShakeBase> _shakePattern, float _scale /*= 1.f*/)
{
	// プレイヤー取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) { return; }

	// マネージャー取得
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager) { return; }

	// カメラシェイク開始
	CameraManager->StartCameraShake(_shakePattern, _scale);
}


/*
 * 関数名　　　　：PlaySound()
 * 処理内容　　　：音声再生
 * 引数１　　　　：USoundBase* _sound_Obj・・・音声のアセット
 * 引数２　　　　：音声を開始する時間
 * 戻り値　　　　：なし
 */
void UActionAssistComponent::PlaySound(USoundBase* _sound_Obj, float _startTime /*= 0*/)
{
	if (_sound_Obj)
	{
		// 再生(7番目の引き数でオーナーアクターを自分に)
		UGameplayStatics::PlaySound2D(GetWorld(), _sound_Obj, 1.0f, 1.0f, _startTime, nullptr, GetOwner());
	}
}


/*
 * 関数名　　　　：GetFrontActor()
 * 処理内容　　　：敵が直線状にいるか判定
 * 戻り値　　　　：見つけたActorの情報を返す
 */
AActor* UActionAssistComponent::GetFrontActor()
{
	// 所有者の情報取得
	AActor* pOwner = GetOwner();

	// 見つけた敵を格納
	AActor* HitEnemy;

	if (pOwner)
	{
		// オーナーがいない場合null
		HitEnemy = nullptr;
	}
	else
	{
		// レイを飛ばす
		// 飛ばす方向指定
		float Rotation_Z = pOwner->GetActorRotation().Yaw;
		float LineDirection = (Rotation_Z > 0) ? (RayLength_CorrectAngle) : (-RayLength_CorrectAngle);

		// 始点
		FVector start = pOwner->GetActorLocation();

		// 終点
		FVector end = FVector(start.X, start.Y + LineDirection, start.Z);

		// 自身を除く
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(pOwner);

		// 当たったオブジェクト格納用
		FHitResult OutHit;

		// ヒットした場合true
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);

		// ヒットしていたらActorを格納していなかったらnull
		HitEnemy = (isHit) ? (OutHit.GetActor()) : (nullptr);
	}

	return HitEnemy;
}


/*
 * 関数名　　　　：OwnerParallelToCamera()
 * 引数			 ：bool _lookRight・・・現在右を向いているか
 * 処理内容　　　：オーナーをカメラに向ける
 * 戻り値　　　　：なし
 */
void UActionAssistComponent::OwnerParallelToCamera(bool _lookRight)
{
	// プレイヤーコントローラー取得
	ANEOPlayerController* PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));;
	if (!PlayerController) { return; }

	// カメラの角度を取得
	const FRotator CameraRotation = PlayerController->GetNowCameraRotation();

	// カメラのピッチとヨー
	double Camera_Pitch = CameraRotation.Pitch;
	double Camera_Yaw = CameraRotation.Yaw;

	// 新しい角度取得
	const double NewAngle_Pitch = GetAngle_Pitch(_lookRight, Camera_Pitch);
	const double NewAngle_Yaw = GetAngle_Yaw(_lookRight, Camera_Yaw);

	FRotator NewAngle = FRotator(0.0, NewAngle_Yaw, NewAngle_Pitch);

	// 回転
	GetOwner()->SetActorRotation(NewAngle);
}


/*
 * 関数名		：GetAngle_Yaw()
 * 引数１		：bool _lookRight・・・現在右を向いているか
 * 引数２		：double _camera_Yaw・・・カメラの向き
 * 処理内容		：次の角度取得
 * 戻り値		：次の角度
 */
double UActionAssistComponent::GetAngle_Yaw(bool _lookRight, double _camera_Yaw)
{
	// 新しい角度判定
	double NewAngle = (!_lookRight)?(_camera_Yaw + DIRECTION_Y):(_camera_Yaw - DIRECTION_Y);

	return NewAngle;
}


/*
 * 関数名		：GetAngle_Pitch()
 * 引数１		：bool _lookRight・・・現在右を向いているか
 * 引数２		：double _camera_Pitch・・・カメラの向き
 * 処理内容		：次の角度取得
 * 戻り値		：次の角度
 */
double UActionAssistComponent::GetAngle_Pitch(bool _lookRight, double _camera_Pitch)
{
	// 機能のオンオフ
	if (!bUseFaceCamera) { return 0.0; }

	// 新しい角度判定
	double NewAngle = (!_lookRight)?(_camera_Pitch):(-_camera_Pitch);

	return NewAngle;
}
