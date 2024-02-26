
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "NEO/WeaponSystem/WeaponBase.h"

#include "CharacterBase.generated.h"



//-----------------キャラクターのステータス-------------------------------------------------
USTRUCT(BlueprintType)
struct FCharacterStatus
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 HP;								// ヒットポイント

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MoveSpeed;						// 移動速度

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float JumpHeight;						// ジャンプ力

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ComboDamageFactor;				// コンボ時のダメージ倍率


	// 初期化関数
	FCharacterStatus InitParam(int32 _hp,float _moveSpeed, float _jumpHeight, float _comboDamageFactor)
	{
		FCharacterStatus DefaultStatus;

		DefaultStatus.HP = _hp;
		DefaultStatus.MoveSpeed = _moveSpeed;
		DefaultStatus.JumpHeight = _jumpHeight;
		DefaultStatus.ComboDamageFactor = _comboDamageFactor;
		return DefaultStatus;
	}
};
//----------------------------------------------------------------------------------------

UCLASS()
class NEO_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 攻撃処理
	virtual void Attack() { return; }

	// 被ダメージ処理
	virtual void TakedDamage(float _damge, bool _bLastAttack = false) { return; }

	// 前方後方になにかあるかチェック
	bool WallCheck(float _lineLength);

protected:

	// 移動処理
	virtual void Move() { return; }
	virtual void Move(const FInputActionValue& _value) { return; }

	// ジャンプ処理
	virtual void Jump() { return; }
	virtual void Jump(float _deltaTime) { return; }

	// ヒットストップ処理
	void HitStop(float _speedDuringHitStop, float _stopTime);

	// ステータス設定
	void InitStatus(int32 _hp, float _moveSpeed, float _jumpHeight, float _comboDamageFactor);

	// ステータス取得
	FCharacterStatus GetStatus()const { return Status; }

	// 何らかのアニメーションが再生中かどうか
	bool IsAnimationPlaying()const;

	// 指定したパスのアニメーションアセットを返す
	UAnimMontage* GetAnimationAsset(TCHAR* _animAssetPath);

private:

	// ヒットストップ終了処理
	void EndHitStop();

protected:

	//-----------------コンポーネント変数--------------------------------------

	// アクションのアシスト用コンポーネント
	UPROPERTY(EditAnywhere, Category = "Action Assist")
		TObjectPtr<class UActionAssistComponent> ActionAssistComp;

	//--------------------------------------------------------------------------

private:

	// キャラクターの基本ステータス
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FCharacterStatus Status;

	// タイマーハンドル
	FTimerHandle TimerHandle_HitStop;
};