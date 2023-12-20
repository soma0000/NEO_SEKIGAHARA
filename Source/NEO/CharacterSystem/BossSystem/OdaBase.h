// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPOdaEnum.h"
#include "GameFramework/CharacterMovementComponent.h"			//�L�����N�^�[���[�u�����g���g�����߂ɕK�v�ȃw�b�_�[
#include "Kismet/GameplayStatics.h"								//getplayerpawn�����g�����߂ɕK�v�ȃw�b�_�[
#include "Kismet/KismetSystemLibrary.h"							//printString���g�����߂ɕK�v�ȃw�b�_�[
#include "Animation/AnimMontage.h"								//�A�j���[�V�����𗬂����߂ɕK�v�ȃw�b�_�[
#include "Math/UnrealMathUtility.h"								//RandRange���g�����߂ɕK�v�ȃw�b�_�[
#include "Math/Vector.h"										//Dist(����)���g�����߂ɕK�v�ȃw�b�_�[
#include "Components/BoxComponent.h"							//�{�b�N�X�R���|�[�l���g���g������
#include "Components/CapsuleComponent.h"						//�J�v�Z���R���|�[�l���g���g������
#include "Templates/SubclassOf.h"								//TSubclassOf���g�����߂ɕK�v�ȃw�b�_�[
#include "NEO/CharacterSystem/PlayerSystem/PlayerCharacter.h"	//�v���C���[�L�������擾���邽�߂ɕK�v�ȃw�b�_�[
#include "NiagaraComponent.h"									//�i�C�A�K���G�t�F�N�g���ĂԂ��߂ɕK�v�ȃw�b�_�[
#include "NiagaraFunctionLibrary.h"								//�i�C�A�K���G�t�F�N�g���ĂԂ��߂ɕK�v�ȃw�b�_�[
#include "NEO/CharacterSystem/ActionAssistComponent.h"
#include "NEO/CharacterSystem/CharacterBase.h"

#include "OdaBase.generated.h"

class AWeaponBase;

USTRUCT(BlueprintType)
struct FEnemyInfo {
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> Enemy;

	UPROPERTY(EditAnywhere)
		float BossHPTiming;

	UPROPERTY(EditAnywhere)
		FVector SpawnPoint;

	UPROPERTY(EditAnywhere)
		bool isSpawn;
};

//�ʏ�̈ړ����x
USTRUCT(BlueprintType)
struct FBossSpeed {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Flont;

	UPROPERTY(EditAnywhere)
	float Dash;

	UPROPERTY(EditAnywhere)
	float Back;
};

UCLASS()
class NEO_API AOdaBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOdaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�֐�-------------------------------------------------------------------------------------------------------------------

	//�{�X�̃X�e�[�g�ł̏���----------------
	void OdaStay1(int Timer);

	void OdaMovie(int Timer);

	void OdaAttack1(int Timer);

	void OdaAttack2(int Timer);

	void OdaUlt(int Timer);
	//------------------------------------

	//�{�X�̓����̏���
	void OdaMove1(int DeltaTime, int StopTimer);

	void IsWallCheck(ECPPOdaEnum BossState);

	//�ϐ�---------------------------------------------------------------------------------------------

	//�X�|�[��������--------------------------------------------------------------------------
	//�X�|�[�����̎~�܂�Ƃ����ǂ���
	UPROPERTY()
		bool SpawnDelay;

	//�X�|�[�����~�܂鎞��
	UPROPERTY()
		int SpawnTimer;
	//----------------------------------------------------------------------------------------


	//�J�E���^�[------------------------------------------------------------
	//���Ԃ̎擾
	int FlameCounter;

	//int�^�̃J�E���^�[
	int TimeCounter;
	//---------------------------------------------------------------------

	//�{�X�������Ă������ǂ���
	UPROPERTY(BlueprintReadOnly)
		bool isMove;

	//�{�X�ƃv���C���[�Ƃ̋���----------------------------
	//X��
	//�{�X
	FVector BossPosX;
	//�v���C���[
	FVector PlayerPosX;
	//Y��
	//�{�X
	FVector BossPosY;
	//�v���C���[
	FVector PlayerPosY;
	//---------------------------------------------------

public:
	//�񋓌^
	UPROPERTY(EditAnywhere)
		ECPPOdaEnum OdaMoveEnum;

protected:

	ECPPOdaEnum HoldOdaMoveEnum;


	//�ߐڍU�����������ăv���C���[���悯��Ȃ��̂ŏ����x��������
	UPROPERTY()
		int Attack1Delay;

	//random�̒l������ׂ̕ϐ�
	UPROPERTY()
		int RandomNum;

	//�U�����U������Ȃ��悤��
	bool isNowAttacking = false;

public:

	//���̏����擾
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sword", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* swordConp;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�R���|�[�l���g-----------------------------------------------------------------------------------------------------
	//�V�[���R���|�[�l���g
	UPROPERTY()
		USceneComponent* Parent;

	//�v���C���[�L�����N�^�[
	UPROPERTY()
		AActor* PlayerChara;

	//���̃R���W����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoxComp", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComp;

	//�L�����N�^�[���[�u�����g
	UPROPERTY()
		UCharacterMovementComponent* NobunagaMovement;
	//--------------------------------------------------------------------------------------------------------------------

	UPROPERTY()
		ECPPOdaState isModeNow;
	//UFUNCTION()
	//void NomalMode();
	//UFUNCTION()
	//void UltMode();


	//�_���[�W����-----------------------------------------------------
	UFUNCTION()
		virtual void TakedDamage(float Damage , bool _bLastAttacked = false);

	// �U���������������̏���
	virtual void Attack_Sword(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation);	// ���̏���


	UFUNCTION()
		void WorldTimeReturn();

	//�_���[�W�^�C�v�N���X
	UPROPERTY()
		TSubclassOf < class UDamageType > DamageTypeClass;

	//�Ռ��g�̃_���[�W�����邽�߂̕ϐ�
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		float BladeDamage;

	//�ʏ�̏Ռ��g�̃_���[�W
	UPROPERTY(EditAnywhere, Category = "Damage")
		float ShockWaveDamage;

	//�K�E�Z���̏Ռ��g�̃_���[�W
	UPROPERTY(EditAnywhere, Category = "Damage")
		float UltShockWaveDamage;

	//�K�E�Z�̃_���[�W
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float UltDamage;

	//�_���[�W������������ǂ���
	UPROPERTY()
		bool isDamageHit = false;

	//----------------------------------------------------------------


	//���[�g���[�V����(�^��)�̍쐬
	UFUNCTION()
		void setRootMotionMove(float speed);


	//���x----------------------------------
	UPROPERTY(EditAnywhere)
		FBossSpeed OdaSpeed;
	//--------------------------------------


	//�Ռ��g-----------------------------------------------------------------------------
	//�������Ռ��g���o�����߂̕ϐ�
	UPROPERTY()
		bool OneMoreShockWave;

	//�s���ύX���Ԑݒ�
	UPROPERTY(EditAnywhere)
		int ChangeFlontTimer;

	//���[�V��������񂾂��������߂̕ϐ�
	struct MotionPlaying
	{
		bool isKnockbackPlay;
		bool isAttack1Play;
		bool isAttack2Play;
		bool isUltPlay;
		bool isDeathPlay;
		bool isMoviePlay;
	};
	MotionPlaying isMotionPlaying;
	//����������悤�̊֐�
	void isMotionReset(bool _isPlaying);

	//�Ռ��g�̏o���^�C�~���O�̒���
	UPROPERTY()
		bool isShockWaveSpawnTiming;

	//�Ռ��g�̏o���^�C�~���O�̒������邽�߂̊֐�
	UFUNCTION()
		void ShockWaveSpawnFlagChange();

	//�Ռ��g���o��������ׂ̕ϐ�
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor>ShockWaveSpawn;
	//-----------------------------------------------------------------------------------

	//�K�E�Z---------------------------------------------------------------------------
	//�K�E�Z�̏o���^�C�~���O�̒���
	UPROPERTY()
		bool isUltSpawnTiming;

	//�K�E�Z��ł^�C�~���O�̒���
	UPROPERTY(BlueprintReadOnly)
		bool isUltShotTiming;

	//�K�E�Z�̏o���^�C�~���O�̒������邽�߂̊֐�
	UFUNCTION()
		void UltSpawnFlagChange(TSubclassOf<AActor>_UltSpawn);

	//�K�E�Z���o��������ׂ̕ϐ�
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor>UltSpawn;

	//�K�E�Z�͈�񂾂��o�������̂Ńt���O�ŊǗ�
	UPROPERTY()
		bool isUltShot;

	//�K�E�Z���������ł��߂̃J�E���^�[
	UPROPERTY()
		int UltTimer;

	//------------------------------------------------------------------------------

	//�U���̃t���O
	UPROPERTY(BlueprintReadOnly)
		bool IsAttackNow;

	//�U���̃t���O��؂�ւ���ׂ̊֐�
	UFUNCTION()
		void AttackFlagChange();

	//�U�����ꂽ�񐔂���萔��������ߐڍU�����ςȂ��ׂ̕ϐ�
	UPROPERTY()
		bool isNotAttackNow;

	//��萔�̌v��
	UPROPERTY()
		int NotAttackCount;

	//�q�b�g�G�t�F�N�g�ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UNiagaraSystem* HitParticles;

	//HP���A���Ō���Ȃ��悤�Ƀ��b�N
	UPROPERTY()
		bool isHPLock;

	//HP���A���Ō���Ȃ��悤�Ƀ��b�N���邽�߂̊֐�
	UFUNCTION()
		void HPLock();

	//�v���C���[�̕����Ɍ����鏈��
	UFUNCTION()
		void ToPlayerRotate();

	//�������U���̃R���{
	UFUNCTION()
		void Is2Combo();

	//�R���{�������ǂ����őҋ@���Ԃ��ς��̂Ń^�C�}�[��p��
	UPROPERTY()
		int Attack1WaitTimer;

	//�ߐڂ̑ҋ@����
	UPROPERTY(EditAnywhere)
		int Attack1WaitingTime;

	//�ҋ@���邩�ǂ���
	bool isAttack1Waiting;

	//�ߐڃR���{�����i�ڂ�
	UPROPERTY()
		int Combo1Counter;

	//�������R���{�����i�ڂ�
	UPROPERTY()
		int Combo2Counter;

	//�_���[�W���Z�����l��ۑ����Ă����ϐ�
	float HoldDamageAdd;

	//�v���C���[���m�b�N�o�b�N�����邽�߂ɍŌ�̍U�����ǂ������Ƃ�֐�
	UFUNCTION(BlueprintCallable)
		bool LastAttack();

	UPROPERTY()
		bool isBossHPRock;

	UFUNCTION()
		void BossHPRock();

	//�񋓌^��Stay�ɖ߂��ׂ̊֐�
	UFUNCTION()
		void BacktoStayCase();


	//���[�V����--------------------------------------------------------------------------------------------------
	//�̂����郂�[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossKnockMontage;		//�����F�ϐ�������AnimMontage_�͕K�{�炵��

	//�U�����鉼���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossAttack1;

	//�Ռ��g�U�����鉼���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossAttack2;

	//�Ռ��g�U�����鉼���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossUltimate;

	//�ӂ��Ƃԃ��[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossBlowAway;

	//���S���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossDeath;

	//���[�r�[�����[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossStartMovie;

	//-------------------------------------------------------------------------------------------------------------

	//�A�j���[�V�����̒��������
	struct AnimTime {
		int BossKnock;
		int Attack1;
		int Attack2;
		int Ultimate;
		int BlowAway;
		int Death;
		int StartMovie;
	};

	AnimTime iAnimTime;

	//�X�^�[�g���[�r�[�A�j���[�V��������񂾂������ׂ̕ϐ�
	bool bStartMovieOnce;

	//���[�r�[���ɓ������t���O���Ǘ�����ϐ�
	bool MovieMove;

	//�X�^�[�g���[�r�[�𗬂����ǂ����̊֐�
	void InMovie();

	//�A�j���[�V����������ׂ̊֐�
	void AnimationPlay(class UAnimMontage* AnimMontage_AnimPlay, bool* _isMotionPlay);

	//�m�b�N�o�b�N���Ăӂ��ƂԊ֐�
	UFUNCTION()
		void BossKnockback();

	FTimerHandle TimerHandle_DeathToGameOver;

	FTimerHandle TimerHandle_KnockbackAnimationFinish;
	//���[�r�[�p�A�j���[�V�����I�������邽�߂ɕK�v�ȃn���h��
	FTimerHandle TimerHandle_MovieAnimationFinish;

	//�㉺�ړ�
	UFUNCTION()
		void BossMoveX(float Speed);

	//�O���ړ�
	UFUNCTION()
		void BossMoveY(float Speed);

	//�_���[�W�l
	UPROPERTY(EditAnywhere, Category = "Damage")
		float SwordFirstDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float SwordAddDamage;

	//�G���G���Ăԏ���
	UFUNCTION(BlueprintCallable)
		void CallEnemy(int _element);

	//�G���G���Ăԏ�������񂾂��������ׂ̕ϐ�
	int EnemyOneTimeSpawn;

	//�e�������G�̃p�X
	FString GunPath = "/Game/0102/Enemy/Gunman/MyGunMan3.MyGunMan3_C"; // /Content �ȉ��̃p�X�� /Game �ȉ��̃p�X�ɒu�������

	//��莝�����G�̃p�X
	FString LancePath = "/Game/0102/Enemy/Lancer/BP_LancerTag3.BP_LancerTag3_C"; // /Content �ȉ��̃p�X�� /Game �ȉ��̃p�X�ɒu�������

	//���������G�̃p�X
	FString SwordPath = "/Game/0102/Enemy/Soldier/BP_SoldierTag3.BP_SoldierTag3_C"; // /Content �ȉ��̃p�X�� /Game �ȉ��̃p�X�ɒu�������


	//�G�l�~�[�̏��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TArray<FEnemyInfo> Enemy;
	//�G���o���A�폜���邽�߂̔z��
	TArray<AActor*> EnemyActor;

	//void EnemySpawnPoint(int _PointCount);

		//�{�b�N�X�R���|�[�l���g�̃I�[�o�[���b�v����
		UFUNCTION()
		void CheckOverlap();

	//�v���C���[----------------------------------------------------------------------
	//�v���C���[�ɓ���������
	UFUNCTION()
		void PlayerOnOverlap(/*FHitResult& _HitResult*/);

	//�v���C���[�ɑ΂��Ă�HP���b�N
	UPROPERTY()
		bool bIsAttacked;

	//�v���C���[�ɑ΂��Ă�HP���b�N���邽�߂̊֐�
	UFUNCTION()
		void toPlayerAttacked();
	//-----------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////
//���S����
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
		bool IsAreaEnemy = false;

	void Death();

	UFUNCTION(BlueprintCallable)
		float GetHP()const { return Health; }

	//�̗�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Health;

	UPROPERTY(EditAnywhere)
		float MaxHealth;

protected:
	AActor* GetPlayer();
};
