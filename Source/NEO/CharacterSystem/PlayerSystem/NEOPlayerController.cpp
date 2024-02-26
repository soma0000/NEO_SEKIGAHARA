// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/NEOGameMode.h"
#include "PlayerBase.h"

ANEOPlayerController::ANEOPlayerController()
	: DefaultRemainingLives(2)
	, RespownPosHeight(300.f)
	, RemainingLives(DefaultRemainingLives)
	, PlayerIsDead(false)
{

}

// �Q�[���J�n���̏���
void ANEOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// �Q�[�����[�h�擾
	pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// �v���C���[�擾
	pPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

	//ConsoleCommand("r.SetRes 45�~25w");		// Steam Dech�p
	//ConsoleCommand("r.SetRes 45�~25f");

	//ConsoleCommand("r.SetRes 3840�~2160w");	// 4K�Ή��p
	//ConsoleCommand("r.SetRes 3840�~2160f");

	ConsoleCommand("r.SetRes 1920�~1080w");		// �ʏ�
	ConsoleCommand("r.SetRes 1920�~1080f");
}


/*
 * �֐����@�@�@�@�FResetPlayerStatus()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�ړ�����)
 * �����P�@�@�@�@�FFInputActionValue& Value�E�E�E���͗�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOPlayerController::ResetPlayerStatus()
{
	// �c�@�ݒ�
	RemainingLives = DefaultRemainingLives;
}


/*
 * �֐����@�@�@�@�FDestroyPlayer()
 * �������e�@�@�@�F�v���C���[���폜
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOPlayerController::DestroyPlayer()
{
	// �v���C���[���Q�[�����[�h�̏�񂪎��Ă��Ȃ��Ƃ�
	if (!pPlayer || !pGameMode) { return; }

	// �v���C���[�폜
	pPlayer->Destroy();

	// �v���C���[��NULL��
	pPlayer = nullptr;

	// �v���C���[�����S��Ԃ�
	PlayerIsDead = true;

	// �Q�[�������̏�Ԃ�
	pGameMode->SetNextGameState();
}


/*
 * �֐����@�@�@�@�FRespawnPlayer()
 * �������e�@�@�@�F�v���C���[�����X�|�[��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOPlayerController::RespawnPlayer()
{
	// �v���C���[���Q�[�����[�h�̏�񂪎��Ă��Ȃ��Ƃ�
	if (!pPlayer || !pGameMode) { return; }

	// �v���C���[�����S�����ʒu�擾
	FTransform RespownTransForm = pPlayer->GetActorTransform();

	// �v���C���[���폜
	pPlayer->Destroy();

	// �v���C���[��NULL��
	pPlayer = nullptr;

	// �v���C���[�̎c�@��1���炷
	ReduceRemainingLives();

	// �����ʒu����������
	const FVector RespownPos = RespownTransForm.GetTranslation();
	RespownTransForm.SetTranslation(FVector(RespownPos.X, RespownPos.Y, RespownPos.Z + RespownPosHeight));

	// �V�����v���C���[�𐶐�
	pPlayer = Cast<APlayerBase>(GetWorld()->SpawnActor<APawn>(pGameMode->GetDefaultPawnClass(), RespownTransForm));

	// ���X�|�[���̃J��������
	SetViewTargetWithBlend(pGameMode->GetNowPlayerCamera(), 0.f);
}


/*
 * �֐����@�@�@�@�FGetPlayerLocation()
 * �������e�@�@�@�F�v���C���[�̍��W��Ԃ�
 * �߂�l�@�@�@�@�F�v���C���[�̍��W
 */
FVector ANEOPlayerController::GetPlayerLocation()const
{
	// �v���C���[�̏�񂪎��Ă��Ȃ��Ƃ�
	if (!pPlayer){ return FVector::ZeroVector; }

	// �v���C���[�̍��W��Ԃ�
	return pPlayer->GetActorLocation();
}

/*
 * �֐����@�@�@�@�FGetNowCameraRotation()
 * �������e�@�@�@�F�J�����̉�]
 * �߂�l�@�@�@�@�F�Ȃ�
 */
FRotator ANEOPlayerController::GetNowCameraRotation()const
{
	if (pGameMode)
	{
		// ���݂̃J�������擾
		AActor* NowCamera = pGameMode->GetNowPlayerCamera();

		// ��]���擾���ĕԂ�
		if (NowCamera)
		{
			// �J�����̃R���|�[�l���g�擾
			UCameraComponent* CameraComponent = NowCamera->FindComponentByClass<UCameraComponent>();
			if (CameraComponent)
			{
				return CameraComponent->GetComponentRotation();
			}
		}
	}

	return FRotator::ZeroRotator;
}


/*
 * �֐����@�@�@�@�FGetIsDebugKeyPressed()
 * �������e�@�@�@�F�f�o�b�O�L�[�ɐݒ肳��Ă���L�[��������Ă��邩
 * �߂�l�@�@�@�@�F�Ȃ�
 */
bool ANEOPlayerController::GetIsDebugKeyPressed()const
{
	for (int i = 0; i < sizeof(KeyNames) / sizeof(FName); ++i)
	{
		if (IsInputKeyDown(KeyNames[i]))
		{
			return false;
		}
	}

	return true;
}