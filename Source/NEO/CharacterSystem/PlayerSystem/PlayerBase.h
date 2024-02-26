// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/CharacterSystem/CharacterBase.h"

#include "PlayerBase.generated.h"

class AWeaponBase;
class UWeaponComponent;
class UCameraShakeBase;
class UDebugComponent;
class ANEOGameMode;
class ANEOPlayerController;
class UNiagaraSystem;

//-----------------inputAction----------------------------------------------------------
USTRUCT(BlueprintType)
struct FMainAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UInputAction> ComboAction1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UInputAction> ComboAction2 = nullptr;
};
//--------------------------------------------------------------------------------------

//-----------------サウンド保管用---------------------------------------------------
USTRUCT(BlueprintType)
struct FAttackSound
{
	GENERATED_BODY()

	// 刀で切る音
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<USoundBase> SwordAttack = nullptr;

	// 槍で殴る音
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<USoundBase> LanceAttack = nullptr;

	// 発砲音
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<USoundBase> Shoot = nullptr;

	// 蹴りの音
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<USoundBase> Kick = nullptr;
};
//----------------------------------------------------------------------------------------

//-----------------アニメーション保管用---------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerAnimation
{
	GENERATED_BODY()

	// コンボ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TObjectPtr<UAnimMontage>> ComboAttack = { nullptr,nullptr };

	// 銃での攻撃
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TObjectPtr<UAnimMontage>> GunAttack = { nullptr,nullptr };

	// 空中にいるときの攻撃
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UAnimMontage> AirAttack = nullptr;

	// 被ダメージ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UAnimMontage> TakeDamage = nullptr;

	// 被ダメージ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UAnimMontage> KnockBack = nullptr;

	// 死亡時
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UAnimMontage> Death = nullptr;
};
//----------------------------------------------------------------------------------------


// プレイヤーの状態を管理するEnum
enum class EPlayerState : uint8
{
	Idle,			// 止まっている状態
	Move,			// 移動
	Jump,			// ジャンプ
	Attack,			// 攻撃
	Damage,			// 被ダメージ
	Death			// 死亡
};


UCLASS()
class NEO_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()

public:

	// コンストラクタ
	APlayerBase();

protected:

	//----------------入力で呼び出される関数-----------------------------
	// 移動
	virtual void Move(const FInputActionValue& _value);

	// 停止
	void Stop();

	// ジャンプ
	virtual void JumpStart();
	virtual void Jump(float _deltaTime);

	// コンボ攻撃
	virtual void ComboAttack(int _attackNum = 0);

	// 一つ目のコンボ
	virtual void AttackStart1();

	// 二つ目のコンボ
	virtual void AttackStart2();

	// 武器種によって攻撃処理
	void Attack_Sword(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation);		// 刀
	void Attack_Lance(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation);		// 槍
	void Attack_Gun();																		// 銃

	// 剣の攻撃アニメーション再生
	void PlayComboAnimtion_Sword(int _attackNum);

	// 槍の攻撃アニメーション再生
	void PlayComboAnimtion_Lance(int _attackNum);

	// 銃の攻撃アニメーション再生
	void PlayComboAnimtion_Gun(int _attackNum);
	//-------------------------------------------------------------------


public:

	//-----------------他クラスで呼び出し可--------------------------------------------------------------------------------------------

	// 攻撃処理
	void Attack();

	// コンボ継続
	void ContinuationCombo();

	// コンボリセット
	void ResetCombo();

	// アニメーションストップ
	void StopMontage();

	// コンボ数を取得
	int GetComboIndex()const { return ComboIndex; }

	// アクションアシストコンポーネントを取得
	UActionAssistComponent* GetActionAssistComponent()const { return ActionAssistComp; }

	// 死亡時のアニメーションの再生を遅くする
	void SlowDownDeathAnimationRate();

	// ダメージ量を返す関数
	UFUNCTION(BlueprintCallable, Category = "Action")
		float GetDamageAmount()const;

	// ダメージを受ける処理
	UFUNCTION(BlueprintCallable, Category = "Action")
		void TakedDamage(float _damage, bool _isLastAttack = false);

	// プレイヤーが地面にいるか
	bool IsPlayerGrounded()const;

	// 接触開始時に行う処理
	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	// デバッグ用関数--------------------

	// ESC:ゲーム終了
	UFUNCTION()
		void QuitGame();

	// F1:ゲームをリスタート
	UFUNCTION()
		void RestartGame();

	// F2:インゲームからスタート
	UFUNCTION()
		void StartInGame();

	// F3:ゲームポーズ
	UFUNCTION()
		void SetGamePause();

	UFUNCTION()
		void ExitBattleArea();

	// F6:無敵のオンオフ
	UFUNCTION()
		void SetAbsolutelyInvincible();

	// F7:死亡処理
	UFUNCTION()
		void SetDeath();
private:

	// キャラクターの回転
	void RotateCharacter(float _nowInput_X);

	// 無敵切り替え
	void SetInvincibilityOn();
	void SetInvincibilityOff();

	// 死亡処理呼び出し
	void CallControllerFunc_DestroyPlayer();

	// 攻撃に関するフラグをすべてリセット
	void ResetAllAttackFlags();

	// プレイヤーの被ダメージ時の処理
	void OnDamage(bool _isLastAttack);

	// プレイヤーの死亡時の処理
	void OnDeath();

	// アニメーション再生
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

protected:

	// ゲーム開始時に呼び出される処理
	virtual void BeginPlay() override;

	// 毎フレーム呼び出される処理
	virtual void Tick(float DeltaTime) override;

	//------------------プレイヤーのセットアップ----------------------------------------------------------------------
	// 入力のセットアップ
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// プレイヤーのデータを初期化
	virtual void InitPlayerData();

	// プレイヤーのステータス初期化
	void SetupPlayerStatus();

	// デバッグキーに関数を割り当てる
	void SetupDebugEventBindings();

protected:

	// プレイやーのアクション管理用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
		FMainAction MainActionMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float InvincibilityTime_Short;			// 短い無敵時間

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float InvincibilityTime_Long;			// 長い無敵時間

	// プレイヤーのアニメーション管理用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		FPlayerAnimation PlayerAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		FAttackSound PlayerSounds;

	// 被ダメージ時のエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		TObjectPtr<UNiagaraSystem> HitEffect;

	// カメラの揺れ方
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UCameraShakeBase> ShakePattern;
	//-------------------------------------------------------------------------------------------------------------


	//-----------------コンポーネント変数--------------------------------------------------------------------------
	
	// デバッグキー設定
	UPROPERTY(EditAnywhere,Category = "Debug")
		TObjectPtr<UDebugComponent> DebugComponent;

	// デバッグキー設定
	UPROPERTY(EditAnywhere,Category = "Weapon")
		TObjectPtr<UWeaponComponent> WeaponComponent;

	//-------------------------------------------------------------------------------------------------------------

	// 死亡アニメーションで倒れてからの再生スピード(1で通常スピード)
	float DeadAnimRate = 0.01f;			

	// 死んでからゲームオーバーまでの時間(秒)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float DeadToGameOverTime = 3.f;		

	// ヒットストップの時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float HitStopSpeed = 0.03f;

	// ヒットストップする時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float HitStopTime = 0.1f;

private:

	// プレイヤーのステート
	EPlayerState State;

	// 蹴り攻撃中かどうか
	bool IsKicking;

	// デバッグ用絶対無敵
	bool AbsolutelyInvincible = false;

	// 無敵状態かどうか
	bool IsInvincibility;

	// フレームカウント用
	float frames;	

	// ジャンプの計算
	const float radPerFrame = 3.14f / 30.f;

	// ジャンプ前の高さ
	float JumpBeforePos_Z;	

	// コンボ継続できるか
	bool CanCombo;							

	// 何段目のコンボか
	int ComboIndex;

	// 何回攻撃されたら武器を落とすか
	int32 DefaultDamageLimit;

	// コンボの段数(First,Second,Third・・・)
	TArray<FName> ComboStartSectionNames;	

	// ハンドル
	FTimerHandle TimerHandle;		

	// ゲームモード保存
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<ANEOGameMode> pGameMode;

	// コントローラー保存
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<ANEOPlayerController> PlayerController;
};