// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NEOGameState.h"
#include "NEOGameMode.generated.h"

#define NUM_OF_EVENT (7)

class UProceduralMeshComponent;
class ANEOGameState;

UCLASS()
class NEO_API ANEOGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ANEOGameMode();

	// �Q�[���J�n���̏���
	void BeginPlay() override;

	// ���t���[���X�V
	void Tick(float DeltaTime) override;

public:

	// �f�t�H���g�|�[���N���X�擾
	TSubclassOf<APawn> GetDefaultPawnClass()const { return DefaultPawnClass; }

	// ���݂̃Q�[���X�e�[�g���擾
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		ANEOGameState* GetGameState()const { return pGameState; }

	// �Q�[�������̏�Ԃֈڂ����������������Ƃ�`����
	void SetNextGameState() { if (pGameState) { pGameState->SetReadyUpdateGame(true); } }

	// �v���C���[�̃J�����̏����ݒ�
	void InitCameraOnPlayer();

	// �v���C���[�̃J�����ݒ�
	void SetPlayerCamera(AActor* _playerCamera) { SplineCamera = _playerCamera; }

	// �v���C���[�̃J������C�ӂ̃J�����ɕύX
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime = 0.f, EViewTargetBlendFunction _blendFunc = VTBlend_Linear, float _blendExp = 0.f, bool _bLockOutgoing = false);

	// ���݂̃J�����擾
	AActor* GetNowPlayerCamera()const;

	// �o�g���G���A���Z�b�g
	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool _IsBattleArea,TArray<ASpawnPoint*> SpawnPoints,
			AActor* Camera,
			UProceduralMeshComponent* LeftMesh,
			UProceduralMeshComponent* RightMesh,
			UProceduralMeshComponent* NearMesh
			, float SetWallDealy = 0.f);

	// �o�g���G���A���쓮�����擾
	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsBattleArea()const { return bIsOnBattleArea; }

	// �ǂ̃R���W�������I����
	void SetWallCollision();

	// �o�g���G���A����o��
	void ExitBattleArea();

	// �C�x���g�i�s�x�擾
	int32 GetEventIndex() { return EventIndex; }

	// �G�̏o������
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);

	// �o�g���G���A���ɓG���o��������
	void SpawnEnemyInBattleArea();

	// �G�l�~�[�̍폜
	void DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy);

	// ��j��C�x���g(�u���[�v�����g����)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void DestroyerEvent();

	//--------------�f�o�b�O�p-----------------
	// �Q�[�����Z�b�g
	void RestartGame();

	// �Q�[���|�[�Y
	void SetGamePause(bool _bPaused);

	// �o�g���G���A���̓G�������폜
	void DestroyBattleAreaEnemy_Debug();
	//-----------------------------------------


private:

	// �o�g���G���A���\�������
	UPROPERTY(VisibleAnywhere)
		TArray< TObjectPtr<UProceduralMeshComponent>> BattleAreaMeshs;

	// �o�g���G���A�̃t���O
	bool bIsOnBattleArea;

	// �o�g���G���A���̓G���i�[
	UPROPERTY(VisibleAnywhere)
		TArray<TObjectPtr<AActor>> Enemies;

	// �G���X�|�[��������|�C���g
	UPROPERTY(VisibleAnywhere)
		TArray<TObjectPtr<ASpawnPoint>> BattleAreaSpawnPoints;

	// �C�x���g�p�̃C���f�b�N�X
	int32 EventIndex;

	// �v���C���[�̃J�����i�[�p(�ړ�����J����)
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AActor> SplineCamera;

	// �v���C���[�̃J����(�J�����Œ莞)
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AActor> pCamera;

	//�o�g���G���A�̃J����
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AActor> BattleAreaCamera;

	// �Q�[���X�e�[�g�̃|�C���^
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<ANEOGameState> pGameState;

	// �v���C���[�R���g���[���[�̃|�C���^
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<class ANEOPlayerController> PlayerController;

	FTimerHandle TimerHandle;
};
