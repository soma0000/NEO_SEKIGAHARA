// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NEOGameState.h"
#include "NEOGameMode.generated.h"

class UProceduralMeshComponent;
class ANEOGameState;

UCLASS()
class NEO_API ANEOGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ANEOGameMode();

	// ゲーム開始時の処理
	void BeginPlay() override;

	// 毎フレーム更新
	void Tick(float DeltaTime) override;

public:

	// デフォルトポーンクラス取得
	TSubclassOf<APawn> GetDefaultPawnClass()const { return DefaultPawnClass; }

	// 現在のゲームステートを取得
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		ANEOGameState* GetGameState()const { return pGameState; }

	// ゲームを次の状態へ移す準備が整ったことを伝える
	void SetNextGameState() { if (pGameState) { pGameState->SetReadyUpdateGame(true); } }

	// プレイヤーのカメラの初期設定
	void InitCameraOnPlayer();

	// プレイヤーのカメラ設定
	void SetPlayerCamera(AActor* _playerCamera) { PlayerCamera = _playerCamera; }

	// プレイヤーにカメラを設定する
	void SetCameraOnPlayer();

	// プレイヤーのカメラを任意のカメラに変更
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime = 0.f, EViewTargetBlendFunction _blendFunc = VTBlend_Linear, float _blendExp = 0.f, bool _bLockOutgoing = false);

	// 現在のカメラ取得
	AActor* GetNowPlayerCamera()const;

	// バトルエリアをセット
	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool _IsBattleArea,TArray<class ASpawnPoint*> SpawnPoints,
			AActor* Camera,
			UProceduralMeshComponent* LeftMesh,
			UProceduralMeshComponent* RightMesh,
			UProceduralMeshComponent* NearMesh
			, float SetWallDealy = 0.f);

	// バトルエリアが作動中か取得
	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsBattleArea()const { return bIsOnBattleArea; }

	// 壁のコリジョンをオンに
	void SetWallCollision();

	// バトルエリアから出る
	void ExitBattleArea();

	// イベント進行度取得
	int32 GetEventIndex() { return EventIndex; }

	// 敵の出現処理
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);

	// バトルエリア内に敵を出現させる
	void SpawnEnemyInBattleArea();

	// エネミーの削除
	void DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy);

	// 柵破壊イベント(ブループリント実装)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void DestroyerEvent();

	//--------------デバッグ用-----------------
	// ゲームリセット
	void RestartGame();

	// ゲームポーズ
	void SetGamePause(bool _bPaused);

	// バトルエリア内の敵を強制削除
	void DestroyBattleAreaEnemy_Debug();
	//-----------------------------------------


private:

	//バトルエリアのカメラ
	AActor* BattleAreaCamera;

	// バトルエリアを構成する壁
	TArray<UProceduralMeshComponent*> BattleAreaMeshs;

	// バトルエリアのフラグ
	bool bIsOnBattleArea;

	// バトルエリア内の敵を格納
	TArray<AActor*> Enemies;

	// 敵をスポーンさせるポイント
	TArray<class ASpawnPoint*> BattleAreaSpawnPoints;

	// イベント用のインデックス
	int32 EventIndex;

	// プレイヤーのカメラ格納用(移動するカメラ)
	AActor* PlayerCamera;

	// プレイヤーのカメラ(カメラ固定時)
	AActor* pCamera;

	// ゲームステートのポインタ
	ANEOGameState* pGameState;

	// プレイヤーコントローラーのポインタ
	class ANEOPlayerController* PlayerController;

	FTimerHandle TimerHandle;
};
