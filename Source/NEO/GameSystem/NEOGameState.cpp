// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "NEO/CharacterSystem/PlayerSystem/NEOPlayerController.h"
#include "NEOGameMode.h"

// コンストラクタ
ANEOGameState::ANEOGameState()
	: GameState(EGameState_NEO::OnNitiden)
	, IsReadyToUpdateGame(false)
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANEOGameState::BeginPlay()
{
	Super::BeginPlay();

	// ウィジェット作成
	NitidenWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.NitidenWidgetClass);
	TitleWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.TitleWidgetClass);
	DemoWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.DemoWidgetClass);
	OpeningWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.OpningWidgetClass);
	OverWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.OverWidgetClass);

	// ゲームの状態初期化
	InitGameState();
}

void ANEOGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGameState(DeltaTime);
}

/*
 * 関数名　　　　：InitGameState()
 * 処理内容　　　：ゲームの状態を初期化
 * 戻り値　　　　：なし
 */
void ANEOGameState::InitGameState()
{
	// コントローラー取得
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// プレイヤーの状態初期化
	PlayerController->ResetPlayerStatus();

	// 日電ロゴ出力
	NitidenWidget->AddToViewport();
}


/*
 * 関数名　　　　：InitInGame()
 * 処理内容　　　：インゲームの状態を更新
 * 戻り値　　　　：なし
 */
void ANEOGameState::InitInGame()
{
	// ゲームモード取得
	ANEOGameMode* pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!pGameMode) { return; }

	// プレイヤーにカメラを設定
	pGameMode->InitCameraOnPlayer();
}

/*
 * 関数名　　　　：ResetGame()
 * 処理内容　　　：インゲームの状態を更新
 * 戻り値　　　　：なし
 */
void ANEOGameState::RestartGame()
{
	// ゲームモード取得
	ANEOGameMode* pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!pGameMode) { return; }

	// プレイヤーにカメラを設定
	pGameMode->RestartGame();
}


// デモ画面に移行する処理
void ANEOGameState::EnterDemoScreen()
{
	if (!DemoWidget || DemoWidget->IsInViewport()) { return; }

	// タイトルロゴ削除
	TitleWidget->RemoveFromParent();

	// タイマーリセット
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	// デモ画面表示
	DemoWidget->AddToViewport();

	// デモ画面に設定
	GameState = EGameState_NEO::OnDemo;
}

/*
 * 関数名　　　　：UpdateGameState()
 * 処理内容　　　：ゲームの状態を更新
 * 引数１　　　　：float DeltaTime・・・毎フレームかかった時間
 * 戻り値　　　　：なし
 */
void ANEOGameState::UpdateGameState(float DeltaTime)
{
	switch (GameState)
	{
	case EGameState_NEO::OnNitiden:
		OnNitiden();
		break;
	case EGameState_NEO::OnTitle:
		OnTitle();
		break;
	case EGameState_NEO::OnDemo:
		OnDemo();
		break;
	case EGameState_NEO::OnOpening:
		OnOpening();
		break;
	case EGameState_NEO::OnGamePlaying:
		OnGamePlaying();
		break;
	case EGameState_NEO::OnGameClear:
		OnGameClear();
		break;
	case EGameState_NEO::OnGameOver:
		OnGameOver();
		break;
	}
}


/*
 * 関数名　　　　：OnNitiden()
 * 処理内容　　　：日電ロゴ出力
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnNitiden()
{
	if (!NitidenWidget || !NitidenWidget->IsInViewport()) { return; }

	// いずれかのキーが押されているか取得
	if (!PlayerController->GetIsAnyKeyPressed()) { return; }

	NitidenWidget->RemoveFromParent();

	// 次のステートへ
	SetNextGameState(EGameState_NEO::OnTitle);
}


/*
 * 関数名　　　　：OnTitle()
 * 処理内容　　　：タイトル画面にいるときの処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnTitle()
{
	if (!TitleWidget->IsInViewport())
	{
		TitleWidget->AddToViewport();

		// デモ画面に移行
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANEOGameState::EnterDemoScreen, DemoTransitionTime, false);

		// タイトルコール
		UGameplayStatics::PlaySound2D(GetWorld(), GameSound.TitleCall, 1.f, 1.f, 0.f, nullptr, this);

		// 環境音(ループ)
		WindAudioComp = UGameplayStatics::SpawnSoundAttached(GameSound.Wind, GetRootComponent());
		FlagAudioComp = UGameplayStatics::SpawnSoundAttached(GameSound.Flag, GetRootComponent());
	}
	else
	{
		// 長押しでタイトル画面がスキップされないように
		if (!PlayerController->GetIsAnyKeyPressed()) { IsReadyToUpdateGame = true; }
		if (!IsReadyToUpdateGame) { return; }
		if (!PlayerController->GetIsAnyKeyPressed()) { return; }
		IsReadyToUpdateGame = false;

		// 表示されているWidgetは消去
		TitleWidget->RemoveFromParent();

		// 音声終了
		WindAudioComp->Stop();
		FlagAudioComp->Stop();

		// タイマーリセット
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		// オープニング再生
		OpeningWidget->AddToViewport();

		// 1秒後に語り再生
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANEOGameState::SpawnKatariSound, 1.f, false);

		// 次のステートへ
		SetNextGameState(EGameState_NEO::OnOpening);
	}
}


/*
 * 関数名　　　　：OnDemo()
 * 処理内容　　　：デモ画面の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnDemo()
{
	if (!DemoWidget || !DemoWidget->IsInViewport()) { return; }

	// いずれかのボタンでタイトル画面に戻る
	if (!PlayerController->GetIsAnyKeyPressed()) { return; }

	// デモ画面削除
	DemoWidget->RemoveFromParent();

	// タイトル画面に設定
	GameState = EGameState_NEO::OnTitle;
}


/*
 * 関数名　　　　：OnOpening()
 * 処理内容　　　：オープニングの処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnOpening()
{
	if (!IsReadyToUpdateGame) { return; }

	if(KatariAudioComp && KatariAudioComp->IsActive()){
		// 語りを終了
		KatariAudioComp->Stop();
	}

	IsReadyToUpdateGame = false;

	// 次のステートへ
	SetNextGameState(EGameState_NEO::OnGamePlaying);

	// インゲーム初期化
	InitInGame();
}


/*
 * 関数名　　　　：OnGamePlaying()
 * 処理内容　　　：ゲームプレイ中の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGamePlaying()
{
	if (!PlayerController) { return; }

	if (!IsReadyToUpdateGame) { return; }

	// プレイヤーが死んでいたらゲームオーバー
	if (PlayerController->GetPlayerIsDead())
	{
		// 次のステートへ
		SetNextGameState(EGameState_NEO::OnGameOver);
	}
	// それ以外はゲームクリア
	else
	{
		// 次のステートへ
		SetNextGameState(EGameState_NEO::OnGameClear);
	}

	IsReadyToUpdateGame = false;
}

/*
 * 関数名　　　　：OnGameClear()
 * 処理内容　　　：ゲームクリア画面の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGameClear()
{
	if (!IsReadyToUpdateGame) { return; }

	// ゲームリセット
	RestartGame();
}


/*
 * 関数名　　　　：OnGameOver()
 * 処理内容　　　：ゲームオーバー画面の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGameOver()
{
	if (!OverWidget || OverWidget->IsInViewport()) { return; }

	OverWidget->AddToViewport();

	if (!IsReadyToUpdateGame) { return; }

	// ゲームリセット
	RestartGame();
}

/*
 * 関数名　　　　：SetNextGameState()
 * 処理内容　　　：ゲームを指定された状態へ
 * 戻り値　　　　：なし
 */
void ANEOGameState::SetNextGameState(EGameState_NEO _nextGameState)
{
	// 指定されたステートへ移動
	GameState = _nextGameState;
}

// 語りの音声再生
void ANEOGameState::SpawnKatariSound()
{
	KatariAudioComp = UGameplayStatics::SpawnSoundAttached(GameSound.Katari, GetRootComponent());
}
