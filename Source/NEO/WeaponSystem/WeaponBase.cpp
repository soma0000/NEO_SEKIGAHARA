// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "NEO/CharacterSystem/ActionAssistComponent.h"

#define JUMP_HEIGHT (PreviousHeight + InitialVelocity * Frames - 0.5f * Gravity * Frames * Frames)
#define JUMP_VELOCITY (InitialVelocity - Gravity * Frames) 
#define DROP_WEAPON_ANGLE (90.0)


// Sets default values
AWeaponBase::AWeaponBase()
	: WeaponState(EWeaponState::PickUp)
	, Damage(0)
	, Frames(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// タグ設定
	Tags.Add("Weapon");

	// アタックアシストコンポーネント作成
	AuraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AuraEffect"));

	// アタックアシストコンポーネント作成
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponState == EWeaponState::Fall)
	{
		BlowsAway(DeltaTime);
	}
}

/*
 * 関数名　　　　：SetupWeaponMesh()
 * 処理内容　　　：プレイヤーのメッシュをセットアップ
 * 引数１　　　　：UStaticMeshComponent*& MeshComp・・・メッシュコンポーネント
 * 引数２　　　　：TCHAR* WeaponAssetPath ・・・・・・・武器のアセットのパス
 * 引数３　　　　：FName PublicName ・・・・・・・・・・エディタでの公開名
 * 戻り値　　　　：なし
 */
void AWeaponBase::SetupWeaponMesh(TObjectPtr<UStaticMeshComponent>& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// 武器のコンポーネントを作成
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(PublicName);

	if (!MeshComp || !WeaponAssetPath) { return; }

	// 武器のアセット設定
	ConstructorHelpers::FObjectFinder< UStaticMesh > weaponMesh(WeaponAssetPath);

	if (weaponMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(weaponMesh.Object);

		// ルートに設定
		RootComponent = MeshComp;
	}

	// エフェクトを追従
	if (AuraEffect)
	{
		AuraEffect->SetupAttachment(RootComponent);
	}
}

/*
 * 関数名　　　　：SetupWeaponMesh()
 * 処理内容　　　：プレイヤーのメッシュをセットアップ(引数がSkeletalMeshの場合)
 * 引数１　　　　：USkeletalMeshComponent*& MeshComp・・・メッシュコンポーネント
 * 引数２　　　　：TCHAR* WeaponAssetPath ・・・・・・・・武器のアセットのパス
 * 引数３　　　　：FName PublicName ・・・・・・・・・・・エディタでの公開名
 * 戻り値　　　　：なし
 */
void AWeaponBase::SetupWeaponMesh(TObjectPtr<USkeletalMeshComponent>& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// 武器のコンポーネントを作成
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (!MeshComp || !WeaponAssetPath) { return; }

	// 武器のアセット設定
	ConstructorHelpers::FObjectFinder< USkeletalMesh > weaponMesh(WeaponAssetPath);

	if (weaponMesh.Succeeded())
	{
		MeshComp->SetSkeletalMeshAsset(weaponMesh.Object);

		// ルートに設定
		RootComponent = MeshComp;
	}

	// エフェクトを追従
	if (AuraEffect)
	{
		AuraEffect->SetupAttachment(RootComponent);
	}
}


/*
 * 関数名　　　　：AttachToCharacter()
 * 処理内容　　　：キャラクターにくっつける
 * 引数１　　　　：ACharacter* _owner ・・・・親になるキャラクター
 * 引数２　　　　：FName _socketName・・・・・ソケットの名前
 * 戻り値　　　　：なし
 */
void AWeaponBase::AttachToCharacter(ACharacter* _owner, FName _socketName)
{
	// 飛んでいるときは取れない
	if (WeaponState != EWeaponState::PickUp) { return; }

	// 持たれている状態に
	WeaponState = EWeaponState::Held;

	// オーナーを設定
	pOwner = _owner;

	if (AuraEffect)
	{
		// エフェクトを見えなくする
		AuraEffect->SetVisibility(false);
	}

	// キャラクターにアタッチ
	AttachToComponent(_owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), _socketName);
}


/*
 * 関数名　　　　：DetachFromCharacter()
 * 処理内容　　　：キャラクターにくっつける
 * 戻り値　　　　：なし
 */
void AWeaponBase::DetachFromCharacter()
{
	// キャラクターから外す
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld,false));

	// 飛んでいる状態に
	WeaponState = EWeaponState::Fall;

	// オーナーを外す
	pOwner = nullptr;

	// ジャンプ前の高さ取得
	PreviousHeight = GetActorLocation().Z;
}


/*
 * 関数名　　　　：GetHitResults()
 * 処理内容　　　：攻撃が当たったかどうか判定(開始と終了を取らない)
 * 引数１　　　　：TArray<FHitResult>& _outHitResults・・・当たったオブジェクト
 * 戻り値　　　　：当たったかどうか
 */
bool AWeaponBase::GetHitResults(TArray<FHitResult>& _outHitResults)
{
	// 当たり判定を取る範囲
	FVector Start = WeaponCollision->GetComponentLocation();
	FVector End = Start;
	
	return GetHitResults(_outHitResults,Start, End);
}


/*
 * 関数名　　　　：GetHitResults()
 * 処理内容　　　：攻撃が当たったかどうか判定
 * 引数１		 ：TArray<FHitResult>& _outHitResults・・・当たったオブジェクト
 * 引数２　　　　：FVector _start ・・・・・・・・ ・・・・当たり判定をとる範囲(開始地点)
 * 引数３		 ：FVector _end・・・・・・・・・・・・・・当たり判定をとる範囲(終了地点)
 * 戻り値　　　　：当たったかどうか
 */
bool AWeaponBase::GetHitResults(TArray<FHitResult>& _outHitResults,FVector _start, FVector _end)
{
	// 武器本体とオーナーには当たらないようにする
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(pOwner);

	// 当たった部分を取得
	FQuat Rot = WeaponCollision->GetComponentQuat();
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(WeaponCollision->GetScaledCapsuleRadius(), WeaponCollision->GetScaledCapsuleHalfHeight());

	// あたっているか確認
	bool IsHit = GetWorld()->SweepMultiByChannel(_outHitResults, _start, _end, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionShape, CollisionParams);

	return IsHit;
}


/*
 * 関数名　　　　：BlowsAway()
 * 処理内容　　　：武器が落ちて地面に刺さる
 * 戻り値　　　　：なし
 */
void AWeaponBase::BlowsAway(float DeltaTime)
{
	// 現在位置取得
	FVector NowPos = GetActorLocation();

	// ジャンプ前の高さから位置更新
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, JUMP_HEIGHT));
	SetActorLocation(nextPos);

	// フレーム更新
	Frames += DeltaTime * 20.f;

	// 現在の回転取得
	FRotator NowRotation = GetActorRotation();

	// 常に回転
	SetActorRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw, NowRotation.Roll + 10.0));

	// 着地処理
	if (JUMP_VELOCITY > 0.f) { return; }
	if (IsLanding()) { HandleLanding(); }
}


/*
 * 関数名　　　　：IsLanding()
 * 処理内容　　　：着地判定
 * 戻り値　　　　：着地しているか
 */
bool AWeaponBase::IsLanding()
{
	// 始点と終点を指定
	FVector start = GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z - RayLength);

	// 自身を除く
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// 判定
	FHitResult OutHit;
	GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);

	// 地面にヒットした場合true
	AActor* HitActor = OutHit.GetActor();

	bool IsLand = HitActor && HitActor->ActorHasTag("Land");

	return IsLand;
}


/*
 * 関数名　　　　：HandleLanding()
 * 処理内容　　　：着地処理
 * 戻り値　　　　：なし
 */
void AWeaponBase::HandleLanding()
{
	// フレームリセット
	Frames = 0.f;

	// 地面に刺さるように位置と角度を補正
	AdjustLandingPosition();
	AdjustLandingAngle();

	// 落下音再生
	if (DropWeaponSoundObj) { ActionAssistComp->PlaySound(DropWeaponSoundObj); }

	// エフェクトを見えるようにする
	if (AuraEffect) { AuraEffect->SetVisibility(true); }

	// 持たれていない状態にする
	WeaponState = EWeaponState::PickUp;
}


/*
 * 関数名　　　　：AdjustLandingPosition()
 * 処理内容　　　：着地時に位置を補正
 * 戻り値　　　　：なし
 */
void AWeaponBase::AdjustLandingPosition()
{
	FVector NowPos = GetActorLocation();

	SetActorLocation(FVector(NowPos.X, NowPos.Y, NowPos.Z));
}


/*
 * 関数名　　　　：AdjustLandingPosition()
 * 処理内容　　　：着地時にランダムに角度を補正
 * 戻り値　　　　：なし
 */
void AWeaponBase::AdjustLandingAngle()
{
	// 角度の要素数取得
	int AnglesElement = CorrectDropAngles.Num();

	// 現在の角度取得
	FRotator NewAngle = FRotator::ZeroRotator;

	// 用意された角度の中からランダムで決定
	if (WeaponType == EWeaponType::Gun)
	{
		NewAngle.Pitch = (AnglesElement > 0) ? (CorrectDropAngles[FMath::Rand() % AnglesElement]) : (DROP_WEAPON_ANGLE - DROP_WEAPON_ANGLE);
		NewAngle.Yaw = (FMath::RandBool()) ? (DROP_WEAPON_ANGLE) : (-DROP_WEAPON_ANGLE);
	}
	else
	{
		NewAngle.Roll = (AnglesElement > 0) ? (CorrectDropAngles[FMath::Rand() % AnglesElement]) : (DROP_WEAPON_ANGLE - DROP_WEAPON_ANGLE);
		NewAngle.Yaw = (FMath::RandBool()) ? (DROP_WEAPON_ANGLE - DROP_WEAPON_ANGLE) : (DROP_WEAPON_ANGLE + DROP_WEAPON_ANGLE);
	}

	SetActorRotation(NewAngle);
}