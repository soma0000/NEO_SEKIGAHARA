// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NEOGameState.generated.h"

// �Q�[���̏�Ԃ��Ǘ�����Enum
UENUM(BlueprintType)
enum class EGameState_NEO : uint8
{
	OnNitiden		UMETA(DisplayName = "Nitiden"),			// ���d���S
	OnTitle			UMETA(DisplayName = "Title"),			// �^�C�g�����
	OnDemo			UMETA(DisplayName = "Demo"),			// �f�����
	OnOpening		UMETA(DisplayName = "Opening"),			// �I�[�v�j���O
	OnGamePlaying	UMETA(DisplayName = "InGame"),			// �C���Q�[��
	OnGameClear		UMETA(DisplayName = "GameClear"),		// �N���A
	OnGameOver		UMETA(DisplayName = "GameOver")			// �I�[�o�[
};

//-----------------�E�B�W�F�b�g�ۊǗp---------------------------------------------------
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

//-----------------�E�B�W�F�b�g�ۊǗp---------------------------------------------------
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


	// �R���X�g���N�^
	ANEOGameState();

public:

	// �Q�[���J�n���̏���
	void BeginPlay() override;

	// ���t���[���X�V
	void Tick(float DeltaTime) override;

	// �Q�[���̏�Ԃ��X�V
	void UpdateGameState(float DeltaTime);

	// �Q�[���X�e�[�g�X�V����
	UFUNCTION(BlueprintCallable, Category = "GetParam")
		class ANEOPlayerController* GetPlayerController()const { return PlayerController; }

	// �Q�[���X�e�[�g�擾
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		EGameState_NEO GetGameState()const { return GameState; }

	// ���̃X�e�[�g�ֈړ����鏀�����������Ƃ��Ɏg�p
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetReadyUpdateGame(bool _isReadyToUpdateGame){ IsReadyToUpdateGame = _isReadyToUpdateGame; }

	// �Q�[����C�ӂ̏�Ԃ�
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetNextGameState(EGameState_NEO _nextGameState);

private:

	// �Q�[���̏�Ԃ�������
	void InitGameState();	

	// �Q�[���̏�Ԃ����Z�b�g
	void RestartGame();

	// �C���Q�[���J�n���ɂ��鏈��
	void InitInGame();

	// �f����ʂɈڍs���鏈��
	void EnterDemoScreen();

	// ���d���S
	void OnNitiden();

	// �^�C�g���̏���
	void OnTitle();

	// �f����ʂ̏���
	void OnDemo();

	// �I�[�v�j���O�̏���
	void OnOpening();

	// �C���Q�[���̏���
	void OnGamePlaying();

	// �N���A�̏���
	void OnGameClear();

	// �I�[�o�[�̏���
	void OnGameOver();	

	// ���̉����Đ�
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

	// �Q�[���̏�Ԃ��Ǘ�
	EGameState_NEO GameState;

	// �Q�[�������̏�ԂɃA�b�v�f�[�g���鏀�����ł�����
	bool IsReadyToUpdateGame;

	FTimerHandle TimerHandle;

	// �v���C���[�̃R���g���[���[�i�[�p
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<class ANEOPlayerController> PlayerController;
};
