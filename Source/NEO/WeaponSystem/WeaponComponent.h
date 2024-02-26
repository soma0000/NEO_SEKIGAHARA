// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"

#include "WeaponComponent.generated.h"

class AWeaponBase;

UCLASS( ClassGroup=(WeaponComponent), meta=(BlueprintSpawnableComponent) )
class NEO_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// 武器を取得
	bool AttachWeapon(AWeaponBase* _newWeapon, FName _socketName = "None", bool _bDestroy = true);

	// 武器を離す
	void DetachWeapon(bool _bDestroy = true);

	// 武器を持ち替える
	bool ChangeWeapon(TSubclassOf<AWeaponBase> _newWeaponClass, FName _socketName = "None", bool _bDestroy = true);

	// 武器取得
	AWeaponBase* GetWeapon()const { return Weapon; }

	// 武器の攻撃力取得
	int32 GetWeaponDamage()const { return Weapon->GetDamage(); }

	FName GetSocketName(int32 _index)const { return SocketNames[_index]; }

	// 現在の武器の種類取得
	EWeaponType GetWeaponType()const { return Weapon->GetWeaponType(); }

	// 攻撃判定発生
	TTuple<TArray<AActor*>, TArray<FVector>> SetCollision();

protected:

	// Sets default values for this component's properties
	UWeaponComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

private:

	// ゲーム開始時に武器をスポーン
	void SpawnWeapon_BeginPlay();

	// アニメーションインスタンスに武器情報をセット
	void SetWeaponType_AnimInstance();

protected:

	// 武器の指定
	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<AWeaponBase> WeaponClass;

	// 武器のソケットの名前
	UPROPERTY(EditAnywhere, Category = "Weapon")
		TArray<FName> SocketNames;

private:

	// ゲーム開始時に武器を持たせるかどうか
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		bool bIsSpawnWeapon_BeginPlay;

	// 武器クラスのポインタ
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AWeaponBase> Weapon;
};
