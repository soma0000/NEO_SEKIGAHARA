// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "ProceduralMeshComponent.h"
#include "NEO/GameSystem/GameSystem_BattleArea.h"
#include "NEO/GameSystem/SpawnPoint.h"
#include "NEO/CharacterSystem/EnemyBase.h"
#include "NEO/CharacterSystem/PlayerSystem/PlayerBase.h"
#include "NEO/CharacterSystem/PlayerSystem/NEOPlayerController.h"



ANEOGameMode::ANEOGameMode()
	: bIsOnBattleArea(false)
{

}


void ANEOGameMode::BeginPlay()
{
	Super::BeginPlay();

	// �Q�[���X�e�[�g���擾
	pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// �v���C���[�R���g���[���[���擾
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ANEOGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// �Q�[���̏�Ԃ��X�V
	if (pGameState) 
	{
		// �Q�[���̏�ԍX�V
		pGameState->UpdateGameState(DeltaTime);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
		pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
}

// �J�����̏����ݒ�
void ANEOGameMode::InitCameraOnPlayer()
{
	SetViewTargetWithBlend(SplineCamera);
}


/*
 * �֐����@�@�@�@�FSetViewTargetWithBlend()
 * �������e�@�@�@�F���݂̃J�����̏���Ԃ�
 * �����P�@�@�@�@�FAActor* _newViewTarget�E�E�E�V�����J�����̏��
 * �����Q�@�@�@�@�Ffloat _blendTime�E�E�E�E�E�E�؂�ւ��ɂ����鎞��
 * �����R�@�@�@�@�FEViewTargetBlendFunction _blendFunc
 * �����S�@�@�@�@�Ffloat _blendExp
 * �����T�@�@�@�@�Fbool _bLockOutgoing
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameMode::SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime, EViewTargetBlendFunction _blendFunc, float _blendExp, bool _bLockOutgoing)
{
	if (!PlayerController) { return; }

	PlayerController->SetViewTargetWithBlend(_newViewTarget, _blendTime);

	// �J������V�����J������
	pCamera = _newViewTarget;
}


/*
 * �֐����@�@�@�@�FGetNowPlayerCamera()
 * �������e�@�@�@�F���݂̃J�����̏���Ԃ�
 * �߂�l�@�@�@�@�F���݂̃J�����̏��
 */
AActor* ANEOGameMode::GetNowPlayerCamera()const
{
	// �o�g���G���A���ɂ��邩�ǂ����ŃJ������I��
	AActor* NowCamera = (bIsOnBattleArea) ? (pCamera) : (SplineCamera);

	
	return NowCamera;
}


/*
 * �֐����@�@�@�@�FSetIsOnBattleArea()
 * �������e�@�@�@�F�o�g���G���A���N������
 * �����P�@�@�@�@�Fbool _IsBattleArea�E�E�E�E�E�E�E�E�E�E�E�E�E�o�g���G���A�𔭓����邩
 * �����Q�@�@�@�@�FTArray<class ASpawnPoint*> SpawnPoints�E�E�E�o���ꏊ�̔z����
 * �����R�@�@�@�@�FAActor* Camera�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�o�g���G���A�̃J�������
 * �����S�@�@�@�@�FUProceduralMeshComponent* LeftMesh�E�E�E�E�E�o�g���G���A�̕ǂ̏��(��)
 * �����T�@�@�@�@�FUProceduralMeshComponent* RightMesh �E�E�E�E�o�g���G���A�̕ǂ̏��(�E)
 * �����U�@�@�@�@�FUProceduralMeshComponent* NearMesh�E�E�E�E�E�o�g���G���A�̕ǂ̏��(��O)
 * �߂�l�@�@�@�@�F�Ȃ�
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

		//�X�|�[���|�C���g���Q�[���X�e�[�g�ɓo�^����
		BattleAreaSpawnPoints.Reset();
		for (auto spawnPoint : SpawnPoints) {
			BattleAreaSpawnPoints.Add(spawnPoint);
		}
	}

	// �ǂ̃R���W�����Ƀf�B���C��t���Ă�����
	if (SetWallDealy != 0.f)
	{
		// ��莞�Ԍ�ɕǐ���
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(TimerHandle, this, &ANEOGameMode::SetWallCollision, SetWallDealy, false);
	}
	else
	{
		SetWallCollision();
	}

	// �v���C���[�̃J�������Œ�J�����ɕύX
	if (Camera)
	{
		SetViewTargetWithBlend(Camera);
	}

	// �o�g���G���A�ɓG���X�|�[��������.
	SpawnEnemyInBattleArea();

	// �o�g���G���A�̃t���O�I��
	bIsOnBattleArea = _IsBattleArea;

	// �C�x���g�i�s
	++EventIndex;
}


/*
 * �֐����@�@�@�@�FSetWallCollision()
 * �������e�@�@�@�F�ǂ̃R���W�������I���ɂ���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameMode::SetWallCollision()
{
	//�o�g���G���A��L����
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			//-------------------�R���W������L����----------------------
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}
}


/*
 * �֐����@�@�@�@�FExitBattleArea()
 * �������e�@�@�@�F�o�g���G���A���甲���鏈��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameMode::ExitBattleArea()
{
	// ���j��C�x���g
	AGameSystem_BattleArea* Area = Cast<AGameSystem_BattleArea>(GetNowPlayerCamera());
	if (Area)
	{
		Area->ExitAreaEvent();
	}


	//�o�g���G���A�𖳌���
	bIsOnBattleArea = false;
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}


	//�Œ�J�������v���C���[�̃J�����ɕύX
	if (!PlayerController->GetPlayerIsDead())
	{
		if (SplineCamera)
		{
			SetViewTargetWithBlend(SplineCamera, 1.f);
		}
	}
	else
	{
		//Log
		UE_LOG(LogTemp, Warning, TEXT("Player.GetOwner() : <CameraActor> is not found"));
	}
}


/*
 * �֐����@�@�@�@�FSpawnEnemy()
 * �������e�@�@�@�F�G�̏o������
 * �����P�@�@�@�@�F�o���ꏊ�̏��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
AActor* ANEOGameMode::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transform���擾����
	FTransform spawnTransform = spawnPoint->GetTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// �G���X�|�[��
	AActor* spawn_Actor = GetWorld()->SpawnActor<AActor>(spawnPoint->GetSpawnActorClass(), spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation(), SpawnParams);


	// �|�C���^�擾
	AEnemyBase* Enemy = Cast<AEnemyBase>(spawn_Actor);

	if (Enemy) {
		Enemy->SetActorTransform(spawnTransform);
		Enemy->SetIsAreaEnemy(true);	//Flag Set 

		return Enemy;
	}

	return nullptr;
}


/*
 * �֐����@�@�@�@�FSpawnEnemyInBattleArea()
 * �������e�@�@�@�F�o�g���G���A�ɓG���o��������
 * �߂�l�@�@�@�@�F�Ȃ�
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

		// �G�l�~�[���X�|�[��
		AActor* newEnemy = SpawnEnemy(spawnPoint);

		if (newEnemy)
		{
			//�G�𐶐�����
			Enemies.Add(newEnemy);
		}
	}
}


/*
 * �֐����@�@�@�@�FDestroyEnemy()
 * �������e�@�@�@�F�G�̍폜����
 * �����P�@�@�@�@�FAActor* _enemy �E�E�E�E�E�E�E�폜�������G
 * �����Q�@�@�@�@�Fbool _bBattleAreaEnemy�E�E�E�o�g���G���A���̓G���ǂ���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameMode::DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy)
{
	if (!_enemy) { return; }

	// �G���A���̃G�l�~�[��������z�񂩂�폜
	if (_bBattleAreaEnemy)
	{
		Enemies.Remove(_enemy);
	}

	// �G���폜
	_enemy->Destroy();

	// �o�g���G���A���ɓG�����Ȃ��Ȃ����甲����
	if (Enemies.Num() == 0 && bIsOnBattleArea)
	{
		ExitBattleArea();
	}
}


/*
 * �֐����@�@�@�@�FRestartGame()
 * �������e�@�@�@�F�Q�[�����Z�b�g
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(),"GameMap");
}


/*
 * �֐����@�@�@�@�FSetGamePause()
 * �������e�@�@�@�F�Q�[���|�[�Y�̐؂�ւ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameMode::SetGamePause(bool _bPaused)
{
	UGameplayStatics::SetGamePaused(GetWorld(), _bPaused);
}

/*
 * �֐����@�@�@�@�FDestroyBattleAreaEnemy_Debug()
 * �������e�@�@�@�F�����I�Ƀo�g���G���A���̓G���폜
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameMode::DestroyBattleAreaEnemy_Debug()
{
	// �o�g���G���A���̓G�̐��擾
	const int32 EnemyCnt = Enemies.Num();

	// �G���c���Ă��邩����
	if (EnemyCnt == 0) { return; }

	// �G���폜
	for (int i = 0; i < EnemyCnt; ++i)
	{
		// �o�g���G���A���̓G���폜
		DestroyEnemy(Enemies[0], true);
	}
}