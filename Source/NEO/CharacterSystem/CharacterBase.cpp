// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "NEO/WeaponSystem/Gun.h"
#include "NEO/AnimationSystem/CharacterAnimInstance.h"
#include "ActionAssistComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// アタックアシストコンポーネント作成
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


/*
 * 関数名　　　　：HitStop()
 * 処理内容　　　：ヒットストップ
 * 引数１　　　　：float _speedDuringHitStop・・・ヒットストップ中のキャラクターの速度
 * 引数２　　　　：float _stopTime・・・・・・・・ヒットストップを掛ける時間
 * 戻り値　　　　：なし
 */
void ACharacterBase::HitStop(float _speedDuringHitStop, float _stopTime)
{
	// キャラクターのメッシュ取得
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	// キャラクターのメッシュが取得できなかったらリターン
	if (!CharacterMesh) { return; }

	// HitStopを開始
	CharacterMesh->GlobalAnimRateScale = _speedDuringHitStop;

	// HitStopを停止
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_HitStop, this, &ACharacterBase::EndHitStop, _stopTime, false);
}


// 前方後方になにかあるかチェック
bool ACharacterBase::WallCheck(float _lineLength)
{
	if (!this) { return false; }

	// レイキャストを実行する際のパラメータを設定する
	// レイキャストの開始位置はキャラクターの現在位置
	float Rotation_Z = GetActorRotation().Yaw;
	float LineLength = (Rotation_Z > 0) ? (_lineLength) : (-_lineLength);

	// 始点
	FVector start = GetActorLocation();
	// 終点
	FVector end = FVector(start.X, start.Y + LineLength, start.Z);

	FCollisionQueryParams CollisionParams;
	// オーナー自身は除外
	CollisionParams.AddIgnoredActor(this);

	FHitResult HitResult;

	// レイトレースを行う
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_WorldStatic, CollisionParams);


	return IsHit;
}

/*
 * 関数名　　　　：InitCharaStatus()
 * 処理内容　　　：ステータス初期化
 * 戻り値　　　　：なし
 */
void ACharacterBase::InitStatus(int32 _hp, float _moveSpeed, float _jumpHeight, float _comboDamageFactor)
{
	// ステータス初期化
	Status = Status.InitParam(_hp, _moveSpeed, _jumpHeight, _comboDamageFactor);
}


/*
 * 関数名　　　　：EndHitStop()
 * 処理内容　　　：ヒットストップ終了
 * 戻り値　　　　：なし
 */
void ACharacterBase::EndHitStop()
{
	//HitStopを停止
	GetMesh()->GlobalAnimRateScale = 1.f;
}


/*
 * 関数名　　　　：GetAnimationAsset()
 * 処理内容　　　：何らかのアニメーションが再生中かどうか
 * 戻り値　　　　：再生中ならtrue
 */
bool ACharacterBase::IsAnimationPlaying()const
{
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	UAnimMontage* PlayingAnimation = AnimInstance->GetCurrentActiveMontage();

	// インスタンスが見つからなかった場合はfalse
	bool IsPlaying = (!PlayingAnimation)?(false):(true);

	return IsPlaying;
}


/*
 * 関数名　　　　：GetAnimationAsset()
 * 処理内容　　　：アニメーションアセットを返す
 * 引数１　　　　：TCHAR* _animAssetPath ・・・アニメーションアセットのパス
 * 戻り値　　　　：見つかったアニメーション
 */
UAnimMontage* ACharacterBase::GetAnimationAsset(TCHAR* _animAssetPath)
{
	// アセットを探してセット
	ConstructorHelpers::FObjectFinder<UAnimMontage> SearchAnimMontage(_animAssetPath);

	// そのアセットが存在したら格納
	if (SearchAnimMontage.Succeeded())
	{
		UAnimMontage* FoundAnimMontage = SearchAnimMontage.Object;

		return FoundAnimMontage;
	}

	return nullptr;
}