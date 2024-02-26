// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NEOGameState.generated.h"

// ゲームの状態を管理するEnum
UENUM(BlueprintType)
enum class EGameState_NEO : uint8
{
	OnNitiden		UMETA(DisplayName = "Nitiden"),			// 日電ロゴ
	OnTitle			UMETA(DisplayName = "Title"),			// タイトル画面
	OnDemo			UMETA(DisplayName = "Demo"),			// デモ画面
	OnOpening		UMETA(DisplayName = "Opening"),			// オープニング
	OnGamePlaying	UMETA(DisplayName = "InGame"),			// インゲーム
	OnGameClear		UMETA(DisplayName = "GameClear"),		// クリア
	OnGameOver		UMETA(DisplayName = "GameOver")			// オーバー
};

//-----------------ウィジェット保管用---------------------------------------------------
USTRUCT(BlueprintType)
struct FGameWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> NitidenWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> TitleWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> DemoWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> OpningWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> ClearWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> OverWidgetClass;
};
//----------------------------------------------------------------------------------------

//-----------------ウィジェット保管用---------------------------------------------------
USTRUCT(BlueprintType)
struct FGameSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> TitleCall;

	UPROPERTY(EditAnywhere, Category = "Sound")
		TObjectPtr<USoundBase> Wind;

	UPROPERTY(EditAnywhere, Category = "Sound")
		TObjectPtr<USoundBase> Flag;

	UPROPERTY(EditAnywhere, Category = "Sound")
		TObjectPtr<USoundBase> Katari;
};
//----------------------------------------------------------------------------------------


UCLASS()
class NEO_API ANEOGameState : public AGameStateBase
{
	GENERATED_BODY()


	// コンストラクタ
	ANEOGameState();

public:

	// ゲーム開始時の処理
	void BeginPlay() override;

	// 毎フレーム更新
	void Tick(float DeltaTime) override;

	// ゲームの状態を更新
	void UpdateGameState(float DeltaTime);

	// ゲームステート更新処理
	UFUNCTION(BlueprintCallable, Category = "GetParam")
		class ANEOPlayerController* GetPlayerController()const { return PlayerController; }

	// ゲームステート取得
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		EGameState_NEO GetGameState()const { return GameState; }

	// 次のステートへ移動する準備が整ったときに使用
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetReadyUpdateGame(bool _isReadyToUpdateGame){ IsReadyToUpdateGame = _isReadyToUpdateGame; }

	// ゲームを任意の状態へ
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetNextGameState(EGameState_NEO _nextGameState);

private:

	// ゲームの状態を初期化
	void InitGameState();	

	// ゲームの状態をリセット
	void RestartGame();

	// インゲーム開始時にする処理
	void InitInGame();

	// デモ画面に移行する処理
	void EnterDemoScreen();

	// 日電ロゴ
	void OnNitiden();

	// タイトルの処理
	void OnTitle();

	// デモ画面の処理
	void OnDemo();

	// オープニングの処理
	void OnOpening();

	// インゲームの処理
	void OnGamePlaying();

	// クリアの処理
	void OnGameClear();

	// オーバーの処理
	void OnGameOver();	

	// 語りの音声再生
	void SpawnKatariSound();

protected:

	UPROPERTY(EditAnywhere, Category = "Sound")
		TObjectPtr<UAudioComponent> WindAudioComp;

	UPROPERTY(EditAnywhere, Category = "Sound")
		TObjectPtr<UAudioComponent> FlagAudioComp;

	UPROPERTY(EditAnywhere, Category = "Sound")
		TObjectPtr<UAudioComponent> KatariAudioComp;

	UPROPERTY(EditAnywhere, Category = "Sound")
		FGameSound GameSound;

	UPROPERTY(EditAnywhere, Category = "Widget")
		FGameWidget GameWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
		TObjectPtr<UUserWidget> NitidenWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
		TObjectPtr<UUserWidget> TitleWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
		TObjectPtr<UUserWidget> DemoWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
		TObjectPtr<UUserWidget> OpeningWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
		TObjectPtr<UUserWidget> OverWidget;


	UPROPERTY(EditAnywhere, Category = "Time")
		float DemoTransitionTime;

private:

	// ゲームの状態を管理
	EGameState_NEO GameState;

	// ゲームを次の状態にアップデートする準備ができたか
	bool IsReadyToUpdateGame;

	FTimerHandle TimerHandle;

	// プレイヤーのコントローラー格納用
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<class ANEOPlayerController> PlayerController;
};
