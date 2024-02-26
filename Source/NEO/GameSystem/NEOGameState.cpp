// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "NEO/CharacterSystem/PlayerSystem/NEOPlayerController.h"
#include "NEOGameMode.h"

// �R���X�g���N�^
ANEOGameState::ANEOGameState()
	: GameState(EGameState_NEO::OnNitiden)
	, IsReadyToUpdateGame(false)
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANEOGameState::BeginPlay()
{
	Super::BeginPlay();

	// �E�B�W�F�b�g�쐬
	NitidenWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.NitidenWidgetClass);
	TitleWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.TitleWidgetClass);
	DemoWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.DemoWidgetClass);
	OpeningWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.OpningWidgetClass);
	OverWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidget.OverWidgetClass);

	// �Q�[���̏�ԏ�����
	InitGameState();
}

void ANEOGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGameState(DeltaTime);
}

/*
 * �֐����@�@�@�@�FInitGameState()
 * �������e�@�@�@�F�Q�[���̏�Ԃ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::InitGameState()
{
	// �R���g���[���[�擾
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// �v���C���[�̏�ԏ�����
	PlayerController->ResetPlayerStatus();

	// ���d���S�o��
	NitidenWidget->AddToViewport();
}


/*
 * �֐����@�@�@�@�FInitInGame()
 * �������e�@�@�@�F�C���Q�[���̏�Ԃ��X�V
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::InitInGame()
{
	// �Q�[�����[�h�擾
	ANEOGameMode* pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!pGameMode) { return; }

	// �v���C���[�ɃJ������ݒ�
	pGameMode->InitCameraOnPlayer();
}

/*
 * �֐����@�@�@�@�FResetGame()
 * �������e�@�@�@�F�C���Q�[���̏�Ԃ��X�V
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::RestartGame()
{
	// �Q�[�����[�h�擾
	ANEOGameMode* pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!pGameMode) { return; }

	// �v���C���[�ɃJ������ݒ�
	pGameMode->RestartGame();
}


// �f����ʂɈڍs���鏈��
void ANEOGameState::EnterDemoScreen()
{
	if (!DemoWidget || DemoWidget->IsInViewport()) { return; }

	// �^�C�g�����S�폜
	TitleWidget->RemoveFromParent();

	// �^�C�}�[���Z�b�g
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	// �f����ʕ\��
	DemoWidget->AddToViewport();

	// �f����ʂɐݒ�
	GameState = EGameState_NEO::OnDemo;
}

/*
 * �֐����@�@�@�@�FUpdateGameState()
 * �������e�@�@�@�F�Q�[���̏�Ԃ��X�V
 * �����P�@�@�@�@�Ffloat DeltaTime�E�E�E���t���[��������������
 * �߂�l�@�@�@�@�F�Ȃ�
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
 * �֐����@�@�@�@�FOnNitiden()
 * �������e�@�@�@�F���d���S�o��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnNitiden()
{
	if (!NitidenWidget || !NitidenWidget->IsInViewport()) { return; }

	// �����ꂩ�̃L�[��������Ă��邩�擾
	if (!PlayerController->GetIsAnyKeyPressed()) { return; }

	NitidenWidget->RemoveFromParent();

	// ���̃X�e�[�g��
	SetNextGameState(EGameState_NEO::OnTitle);
}


/*
 * �֐����@�@�@�@�FOnTitle()
 * �������e�@�@�@�F�^�C�g����ʂɂ���Ƃ��̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnTitle()
{
	if (!TitleWidget->IsInViewport())
	{
		TitleWidget->AddToViewport();

		// �f����ʂɈڍs
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANEOGameState::EnterDemoScreen, DemoTransitionTime, false);

		// �^�C�g���R�[��
		UGameplayStatics::PlaySound2D(GetWorld(), GameSound.TitleCall, 1.f, 1.f, 0.f, nullptr, this);

		// ����(���[�v)
		WindAudioComp = UGameplayStatics::SpawnSoundAttached(GameSound.Wind, GetRootComponent());
		FlagAudioComp = UGameplayStatics::SpawnSoundAttached(GameSound.Flag, GetRootComponent());
	}
	else
	{
		// �������Ń^�C�g����ʂ��X�L�b�v����Ȃ��悤��
		if (!PlayerController->GetIsAnyKeyPressed()) { IsReadyToUpdateGame = true; }
		if (!IsReadyToUpdateGame) { return; }
		if (!PlayerController->GetIsAnyKeyPressed()) { return; }
		IsReadyToUpdateGame = false;

		// �\������Ă���Widget�͏���
		TitleWidget->RemoveFromParent();

		// �����I��
		WindAudioComp->Stop();
		FlagAudioComp->Stop();

		// �^�C�}�[���Z�b�g
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		// �I�[�v�j���O�Đ�
		OpeningWidget->AddToViewport();

		// 1�b��Ɍ��Đ�
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANEOGameState::SpawnKatariSound, 1.f, false);

		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnOpening);
	}
}


/*
 * �֐����@�@�@�@�FOnDemo()
 * �������e�@�@�@�F�f����ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnDemo()
{
	if (!DemoWidget || !DemoWidget->IsInViewport()) { return; }

	// �����ꂩ�̃{�^���Ń^�C�g����ʂɖ߂�
	if (!PlayerController->GetIsAnyKeyPressed()) { return; }

	// �f����ʍ폜
	DemoWidget->RemoveFromParent();

	// �^�C�g����ʂɐݒ�
	GameState = EGameState_NEO::OnTitle;
}


/*
 * �֐����@�@�@�@�FOnOpening()
 * �������e�@�@�@�F�I�[�v�j���O�̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnOpening()
{
	if (!IsReadyToUpdateGame) { return; }

	if(KatariAudioComp && KatariAudioComp->IsActive()){
		// �����I��
		KatariAudioComp->Stop();
	}

	IsReadyToUpdateGame = false;

	// ���̃X�e�[�g��
	SetNextGameState(EGameState_NEO::OnGamePlaying);

	// �C���Q�[��������
	InitInGame();
}


/*
 * �֐����@�@�@�@�FOnGamePlaying()
 * �������e�@�@�@�F�Q�[���v���C���̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGamePlaying()
{
	if (!PlayerController) { return; }

	if (!IsReadyToUpdateGame) { return; }

	// �v���C���[������ł�����Q�[���I�[�o�[
	if (PlayerController->GetPlayerIsDead())
	{
		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnGameOver);
	}
	// ����ȊO�̓Q�[���N���A
	else
	{
		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnGameClear);
	}

	IsReadyToUpdateGame = false;
}

/*
 * �֐����@�@�@�@�FOnGameClear()
 * �������e�@�@�@�F�Q�[���N���A��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameClear()
{
	if (!IsReadyToUpdateGame) { return; }

	// �Q�[�����Z�b�g
	RestartGame();
}


/*
 * �֐����@�@�@�@�FOnGameOver()
 * �������e�@�@�@�F�Q�[���I�[�o�[��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameOver()
{
	if (!OverWidget || OverWidget->IsInViewport()) { return; }

	OverWidget->AddToViewport();

	if (!IsReadyToUpdateGame) { return; }

	// �Q�[�����Z�b�g
	RestartGame();
}

/*
 * �֐����@�@�@�@�FSetNextGameState()
 * �������e�@�@�@�F�Q�[�����w�肳�ꂽ��Ԃ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::SetNextGameState(EGameState_NEO _nextGameState)
{
	// �w�肳�ꂽ�X�e�[�g�ֈړ�
	GameState = _nextGameState;
}

// ���̉����Đ�
void ANEOGameState::SpawnKatariSound()
{
	KatariAudioComp = UGameplayStatics::SpawnSoundAttached(GameSound.Katari, GetRootComponent());
}
