// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "ProceduralMeshComponent.h"
#include "NEO/GameSystem/SplineCamera.h"
#include "NEO/GameSystem/GameSystem_BattleArea.h"
#include "NEO/GameSystem/SpawnPoint.h"
#include "NEO/CharacterSystem/BossSystem/OdaBase.h"
#include "NEO/CharacterSystem/Enemy/EnamyBase.h"
#include "NEO/CharacterSystem/PlayerSystem/NEOPlayerController.h"

ANEOGameMode::ANEOGameMode()
{

}


void ANEOGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ゲームステートを取得
	pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// プレイヤーコントローラーを取得
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ANEOGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ゲームの状態を更新
	if (pGameState) 
	{
		// ゲームの状態更新
		pGameState->UpdateGameState(DeltaTime);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
		pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
}

// カメラの初期設定
void ANEOGameMode::InitCameraOnPlayer()
{
	SetViewTargetWithBlend(PlayerCamera);
}


/*
 * 関数名　　　　：SetCameraOnPlayer()
 * 処理内容　　　：プレイヤーのカメラ設定
 * 戻り値　　　　：なし
 */
void ANEOGameMode::SetCameraOnPlayer()
{
	if (PlayerController) { return; }

	// プレイヤーが生きていることを確認
	if (!PlayerController->GetPlayerIsDead())
	{
		// バトルエリア内にいるかどうかでカメラを選択
		AActor* NowCamera = (bIsOnBattleArea) ? (pCamera) : (PlayerCamera);

		// プレイヤーのカメラに設定
		SetViewTargetWithBlend(NowCamera);
	}
}


/*
 * 関数名　　　　：SetViewTargetWithBlend()
 * 処理内容　　　：現在のカメラの情報を返す
 * 引数１　　　　：AActor* _newViewTarget・・・新しいカメラの情報
 * 引数２　　　　：float _blendTime・・・・・・切り替えにかける時間
 * 引数３　　　　：EViewTargetBlendFunction _blendFunc
 * 引数４　　　　：float _blendExp
 * 引数５　　　　：bool _bLockOutgoing
 * 戻り値　　　　：なし
 */
void ANEOGameMode::SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime, EViewTargetBlendFunction _blendFunc, float _blendExp, bool _bLockOutgoing)
{
	// プレイヤーコントローラーを取得
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// コントローラーがある時カメラをブレンド
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(_newViewTarget, _blendTime);
	}

	// カメラを新しいカメラへ
	pCamera = _newViewTarget;
}


/*
 * 関数名　　　　：GetNowPlayerCamera()
 * 処理内容　　　：現在のカメラの情報を返す
 * 戻り値　　　　：現在のカメラの情報
 */
AActor* ANEOGameMode::GetNowPlayerCamera()const
{
	// バトルエリア内にいるかどうかでカメラを選択
	AActor* NowCamera = (bIsOnBattleArea) ? (pCamera) : (PlayerCamera);

	if (NowCamera)
	{
		return NowCamera;
	}
	
	return nullptr;
}


/*
 * 関数名　　　　：SetIsOnBattleArea()
 * 処理内容　　　：バトルエリアを起動する
 * 引数１　　　　：bool _IsBattleArea・・・・・・・・・・・・・バトルエリアを発動するか
 * 引数２　　　　：TArray<class ASpawnPoint*> SpawnPoints・・・出現場所の配列情報
 * 引数３　　　　：AActor* Camera・・・・・・・・・・・・・・・バトルエリアのカメラ情報
 * 引数４　　　　：UProceduralMeshComponent* LeftMesh・・・・・バトルエリアの壁の情報(左)
 * 引数５　　　　：UProceduralMeshComponent* RightMesh ・・・・バトルエリアの壁の情報(右)
 * 引数６　　　　：UProceduralMeshComponent* NearMesh・・・・・バトルエリアの壁の情報(手前)
 * 戻り値　　　　：なし
 */
void ANEOGameMode::SetIsOnBattleArea(bool _IsBattleArea,TArray<class ASpawnPoint*> SpawnPoints,
	AActor* Camera = nullptr,
	class UProceduralMeshComponent* LeftMesh = nullptr,
	class UProceduralMeshComponent* RightMesh = nullptr,
	class UProceduralMeshComponent* NearMesh = nullptr,
	float SetWallDealy)
{
	// NULL Check
	if (Camera && LeftMesh && RightMesh && NearMesh) {
		BattleAreaMeshs.Reset();

		BattleAreaMeshs.Add(LeftMesh);
		BattleAreaMeshs.Add(RightMesh);
		BattleAreaMeshs.Add(NearMesh);

		BattleAreaCamera = Camera;

		//スポーンポイントをゲームステートに登録する
		BattleAreaSpawnPoints.Reset();
		for (auto spawnPoint : SpawnPoints) {
			BattleAreaSpawnPoints.Add(spawnPoint);
		}
	}

	// 壁のコリジョンにディレイを付けていたら
	if (SetWallDealy != 0.f)
	{
		// 一定時間後に壁生成
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(TimerHandle, this, &ANEOGameMode::SetWallCollision, SetWallDealy, false);
	}
	else
	{
		SetWallCollision();
	}

	// プレイヤーのカメラを固定カメラに変更
	if (Camera)
	{
		SetViewTargetWithBlend(Camera);
	}

	// バトルエリアに敵をスポーンさせる.
	SpawnEnemyInBattleArea();

	// バトルエリアのフラグオン
	bIsOnBattleArea = _IsBattleArea;

	// イベント進行
	++EventIndex;
}


/*
 * 関数名　　　　：SetWallCollision()
 * 処理内容　　　：壁のコリジョンをオンにする
 * 戻り値　　　　：なし
 */
void ANEOGameMode::SetWallCollision()
{
	//バトルエリアを有効化
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			//-------------------コリジョンを有効化----------------------
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}
}


/*
 * 関数名　　　　：ExitBattleArea()
 * 処理内容　　　：バトルエリアから抜ける処理
 * 戻り値　　　　：なし
 */
void ANEOGameMode::ExitBattleArea()
{
	// 策破壊イベント
	AGameSystem_BattleArea* Area = Cast<AGameSystem_BattleArea>(GetNowPlayerCamera());
	if (Area)
	{
		Area->ExitAreaEvent();
	}


	//バトルエリアを無効化
	bIsOnBattleArea = false;
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}


	//固定カメラをプレイヤーのカメラに変更
	if (!PlayerController->GetPlayerIsDead())
	{
		if (PlayerCamera)
		{
			SetViewTargetWithBlend(PlayerCamera, 1.f);
		}
	}
	else
	{
		//Log
		UE_LOG(LogTemp, Warning, TEXT("Player.GetOwner() : <CameraActor> is not found"));
	}
}


/*
 * 関数名　　　　：SpawnEnemy()
 * 処理内容　　　：敵の出現処理
 * 引数１　　　　：出現場所の情報
 * 戻り値　　　　：なし
 */
AActor* ANEOGameMode::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transformを取得する
	FTransform spawnTransform = spawnPoint->GetTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 敵をスポーン
	AActor* spawn_Actor = GetWorld()->SpawnActor<AActor>(spawnPoint->GetSpawnActorClass(), spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation(), SpawnParams);


	// ポインタ取得
	AOdaBase* Boss = Cast<AOdaBase>(spawn_Actor);
	AEnamyBase* Enemy = Cast<AEnamyBase>(spawn_Actor);

	if (Enemy) {
		Enemy->SetActorTransform(spawnTransform);
		Enemy->IsAreaEnemy = true;

		return Enemy;
	}
	else if (Boss) {
		Boss->SetActorTransform(spawnTransform);
		Boss->IsAreaEnemy = true; 

		return Boss;
	}

	return nullptr;
}


/*
 * 関数名　　　　：SpawnEnemyInBattleArea()
 * 処理内容　　　：バトルエリアに敵を出現させる
 * 戻り値　　　　：なし
 */
void ANEOGameMode::SpawnEnemyInBattleArea()
{
	//Check SpawnPoints
	if (BattleAreaSpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoints is not found"));
		return;
	}

	for (ASpawnPoint* spawnPoint : BattleAreaSpawnPoints) {
		if (!spawnPoint) continue;

		// エネミーをスポーン
		AActor* newEnemy = SpawnEnemy(spawnPoint);

		if (newEnemy)
		{
			//敵を生成する
			Enemies.Add(newEnemy);
		}
	}
}


/*
 * 関数名　　　　：DestroyEnemy()
 * 処理内容　　　：敵の削除処理
 * 引数１　　　　：AActor* _enemy ・・・・・・・削除したい敵
 * 引数２　　　　：bool _bBattleAreaEnemy・・・バトルエリア内の敵かどうか
 * 戻り値　　　　：なし
 */
void ANEOGameMode::DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy)
{
	if (!_enemy) { return; }

	// エリア内のエネミーだったら配列から削除
	if (_bBattleAreaEnemy)
	{
		Enemies.Remove(_enemy);
	}

	// 敵を削除
	_enemy->Destroy();

	// バトルエリア内に敵がいなくなったら抜ける
	if (Enemies.Num() == 0 && bIsOnBattleArea)
	{
		ExitBattleArea();
	}
}


/*
 * 関数名　　　　：RestartGame()
 * 処理内容　　　：ゲームリセット
 * 戻り値　　　　：なし
 */
void ANEOGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(),"GameMap");
}


/*
 * 関数名　　　　：SetGamePause()
 * 処理内容　　　：ゲームポーズの切り替え
 * 戻り値　　　　：なし
 */
void ANEOGameMode::SetGamePause(bool _bPaused)
{
	UGameplayStatics::SetGamePaused(GetWorld(), _bPaused);
}

/*
 * 関数名　　　　：DestroyBattleAreaEnemy_Debug()
 * 処理内容　　　：強制的にバトルエリア内の敵を削除
 * 戻り値　　　　：なし
 */
void ANEOGameMode::DestroyBattleAreaEnemy_Debug()
{
	// バトルエリア内の敵の数取得
	const int32 EnemyCnt = Enemies.Num();

	// 敵が残っているか判定
	if (EnemyCnt == 0) { return; }

	// 敵を削除
	for (int i = 0; i < EnemyCnt; ++i)
	{
		// バトルエリア内の敵を削除
		DestroyEnemy(Enemies[0], true);
	}
}