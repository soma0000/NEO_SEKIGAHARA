// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "NEO/AnimationSystem/CharacterAnimInstance.h"
#include "Gun.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
	:bIsSpawnWeapon_BeginPlay(true)
{
	// Tickを回すかどうか
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ゲーム開始時に武器をスポーン
	SpawnWeapon_BeginPlay();
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


/*
 * 関数名　　　　：AttachWeapon()
 * 処理内容　　　：武器を装備する
 * 引数１　　　　：AWeaponBase* _newWeapon・・・取得する武器
 * 引数２		 ：FName _socketName・・・・・・武器をくっつけるためのソケットの名前
 * 引数３　　　　：bool _bDestroy ・・・・・・・武器を持っている場合離すと同時に削除するか
 * 戻り値　　　　：装備が成功したか
 */
bool UWeaponComponent::AttachWeapon(AWeaponBase* _newWeapon, FName _socketName /*= "None"*/, bool _bDestroy /*= true*/)
{
	// 新しい武器が存在するかつソケットが設定されているか
	if (!_newWeapon || SocketNames.Num() <= 0) { return false; }

	// 武器が取得できる状態か確認
	if (!(_newWeapon->GetIsPickUp())) { return false; }

	// 他の武器を持っているとき武器を離す
	if (Weapon) { DetachWeapon(_bDestroy);}

	// 武器を更新
	Weapon = _newWeapon;

	// AnimationInstanceに武器情報を送る
	SetWeaponType_AnimInstance();

	// 新しくソケットが指定されたら更新する
	const FName NewSocketName = (_socketName != "None") ? (_socketName) : (SocketNames[0]);
	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());

	Weapon->AttachToCharacter(OwnerChara, NewSocketName);

	return true;
}


/*
 * 関数名　　　　：DetachWeapon()
 * 処理内容　　　：当たったオブジェクトをActorに変換
 * 戻り値　　　　：なし
 */
void UWeaponComponent::DetachWeapon(bool _bDestroy /*= true*/)
{
	if (!Weapon) { return; }

	// 持っている武器を離すか削除する
	(!_bDestroy) ? (Weapon->DetachFromCharacter()) : (Weapon->Destroy());

	// 武器を持っていない状態へ
	Weapon = nullptr;

	// AnimationInstanceに武器情報を送る
	SetWeaponType_AnimInstance();
}


/*
 * 関数名　　　　：ChangeWeapon()
 * 処理内容　　　：新しい武器を装備する
 * 引数１　　　　：TSubclassOf<AWeaponBase> _newWeaponClass・・・新しい武器のクラス
 * 引数２		 ：FName _socketName ・・・・・・・・・・・・・・ソケットの名前
 * 引数３　　　　：bool _bDestroy・・・・・・・・・・・・・・・・持ち替える前の武器を削除するかどうか
 * 戻り値　　　　：装備が成功したか
 */
bool UWeaponComponent::ChangeWeapon(TSubclassOf<AWeaponBase> _newWeaponClass, FName _socketName /*= "None"*/, bool _bDestroy /*= true*/)
{
	// 新しい武器が設定されているか確認
	if (!_newWeaponClass) { return false; }

	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerChara;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 武器を持っている場合手離す
	if (Weapon) { Weapon->DetachFromCharacter(); }

	// 装備が成功するかのフラグ
	bool bSucces = false;

	// プレイヤーにアタッチ
	if (Weapon = GetWorld()->SpawnActor<AWeaponBase>(_newWeaponClass, SpawnParams))
	{
		Weapon->AttachToCharacter(OwnerChara, _socketName);

		bSucces = true;
	}

	return bSucces;
}


/*
 * 関数名　　　　：SetCollision()
 * 処理内容　　　：当たったオブジェクトをActorに変換
 * 戻り値　　　　：変換したActorとその座標(タグを持っていないActorは省く)のタプル
 */
TTuple<TArray<AActor*>, TArray<FVector>> UWeaponComponent::SetCollision()
{
	if (!Weapon) 
	{
		TArray<AActor*> NullActor = { nullptr };
		TArray<FVector> ZeroVector = { FVector::ZeroVector };
		return MakeTuple(NullActor, ZeroVector);
	}

	// Actorとその座標格納用
	TArray<AActor*> HitActors;
	TArray<FVector> HitLocations;

	if (Weapon->GetWeaponType() == EWeaponType::Gun)
	{
		// 射撃処理
		Cast<AGun>(Weapon)->Shoot();
		TArray<AActor*> NullActor = { nullptr };
		TArray<FVector> ZeroVector = { FVector::ZeroVector };
	}
	else
	{
		// 武器に当たったオブジェクト取得
		TArray<FHitResult> OutHitResults;
		Weapon->GetHitResults(OutHitResults);

		for (const FHitResult HitResult : OutHitResults)
		{
			// Actor取得
			AActor* HitActor = HitResult.GetActor();

			if (HitActor->Tags.Num() > 0)
			{
				// タグを持っているActorとその座標のみを追加
				HitActors.Add(HitActor);
				HitLocations.Add(HitResult.Location);
			}
		}
	}

	return MakeTuple(HitActors, HitLocations);
}


/*
 * 関数名　　　　：SpawnWeapon_BeginPlay()
 * 処理内容　　　：ゲーム開始時に武器を出現
 * 戻り値　　　　：なし
 */
void UWeaponComponent::SpawnWeapon_BeginPlay()
{
	// ゲーム開始時にスポーンしない設定の時か初期武器(BPから設定可能)が設定されていなかったらスルー
	if (!bIsSpawnWeapon_BeginPlay || !WeaponClass) { return; }

	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerChara;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 武器をスポーンしてキャラクターにアタッチ
	if (Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams))
	{
		// AnimationInstanceに武器情報を送る
		SetWeaponType_AnimInstance();

		// ソケットが設定されているか判定
		if (SocketNames.Num() > 0) { Weapon->AttachToCharacter(OwnerChara, SocketNames[0]);}
	}
}


/*
 * 関数名　　　　：SetWeaponType_AnimInstance()
 * 処理内容　　　：AnimationInstanceに武器情報をセット
 * 戻り値　　　　：なし
 */
void UWeaponComponent::SetWeaponType_AnimInstance()
{
	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());
	if (!OwnerChara) { return; }
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(OwnerChara->GetMesh()->GetAnimInstance());
	if (!AnimInstance) { return; }

	// AnimationInstanceに武器情報を送る
	EWeaponType WeaponType = (!Weapon) ? (EWeaponType::None) : (GetWeaponType());
	AnimInstance->SetWeaponType(WeaponType);
}
