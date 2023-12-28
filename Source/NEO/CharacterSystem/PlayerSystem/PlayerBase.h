// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/CharacterSystem/CharacterBase.h"

#include "PlayerBase.generated.h"

class AWeaponBase;

//-----------------inputAction----------------------------------------------------------
USTRUCT(BlueprintType)
struct FMainAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UInputAction* ComboAction1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UInputAction* ComboAction2 = nullptr;
};
//--------------------------------------------------------------------------------------

//-----------------�T�E���h�ۊǗp---------------------------------------------------
USTRUCT(BlueprintType)
struct FAttackSound
{
	GENERATED_BODY()

	// ���Ő؂鉹
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	USoundBase* SwordAttack = nullptr;

	// ���ŉ��鉹
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* LanceAttack = nullptr;

	// ���C��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* Shoot = nullptr;

	// �R��̉�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* Kick = nullptr;
};
//----------------------------------------------------------------------------------------

//-----------------�A�j���[�V�����ۊǗp---------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerAnimation
{
	GENERATED_BODY()

	// �R���{
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<UAnimMontage*> ComboAttack = { nullptr,nullptr };

	// �e�ł̍U��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* GunAttack = nullptr;

	// �e�ł̍U���Q
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* GunAttack2 = nullptr;

	// �󒆂ɂ���Ƃ��̍U��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* AirAttack = nullptr;

	// ��_���[�W
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* TakeDamage = nullptr;

	// ��_���[�W
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* KnockBack = nullptr;

	// ���S��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* Death = nullptr;
};
//----------------------------------------------------------------------------------------


// �v���C���[�̏�Ԃ��Ǘ�����Enum
enum class EPlayerState : uint8
{
	Idle,			// �~�܂��Ă�����
	Move,			// �ړ�
	Jump,			// �W�����v
	Attack,			// �U��
	Damage,			// ��_���[�W
	Death			// ���S
};


UCLASS()
class NEO_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()

public:

	// �R���X�g���N�^
	APlayerBase();

protected:

	//----------------���͂ŌĂяo�����֐�-----------------------------
	// �ړ�
	virtual void Move(const FInputActionValue& _value);

	// ��~
	void Stop();


	// �W�����v
	virtual void JumpStart();
	virtual void Jump();

	// �R���{�U��
	virtual void ComboAttack(int _attackNum = 0);

	// ��ڂ̃R���{
	virtual void Attack1();

	// ��ڂ̃R���{
	virtual void Attack2();

	// �����ɂ���čU������
	void Attack_Sword(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation);		// ��
	void Attack_Lance(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation);		// ��
	void Attack_Gun();																		// �e

	// ���̍U���A�j���[�V�����Đ�
	void PlayComboAnimtion_Sword(int _attackNum);

	// ���̍U���A�j���[�V�����Đ�
	void PlayComboAnimtion_Lance(int _attackNum);

	// �e�̍U���A�j���[�V�����Đ�
	void PlayComboAnimtion_Gun(int _attackNum);
	//-------------------------------------------------------------------


public:

	//-----------------���N���X�ŌĂяo����--------------------------------------------------------------------------------------------
	// �R���{�p��
	void ContinuationCombo();

	// �R���{���Z�b�g
	void ResetCombo();

	// �A�j���[�V�����X�g�b�v
	void StopMontage();

	// �R���{�����擾
	int GetComboIndex()const { return ComboIndex; }

	// �R��U�������擾
	bool GetKicking()const { return IsKicking; }

	// ���G�؂�ւ�
	void SetInvincibilityOn();
	void SetInvincibilityOff();

	// �A�N�V�����A�V�X�g�R���|�[�l���g���擾
	UActionAssistComponent* GetActionAssistComponent()const { return ActionAssistComp; }

	// ���S���̃A�j���[�V�����̍Đ���x������
	void SlowDownDeathAnimationRate();

	// �_���[�W�ʂ�Ԃ��֐�
	UFUNCTION(BlueprintCallable, Category = "Action")
		float GetDamageAmount()const { return GetWeaponDamage() * (((float)ComboIndex + 1.f) * GetStatus().ComboDamageFactor); }

	// �_���[�W���󂯂鏈��
	UFUNCTION(BlueprintCallable, Category = "Action")
		void TakedDamage(float _damage, bool _isLastAttack = false);

	// �v���C���[���n�ʂɂ��邩
	bool IsPlayerGrounded()const;

	// �ڐG�J�n���ɍs������
	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	// �f�o�b�O�p�֐�--------------------

	// ESC:�Q�[���I��
	UFUNCTION()
		void QuitGame();

	// F1:�Q�[�������X�^�[�g
	UFUNCTION()
		void RestartGame();

	// F3:�Q�[���|�[�Y
	UFUNCTION()
		void SetGamePause();

	UFUNCTION()
		void ExitBattleArea();

	// F6:���G�̃I���I�t
	UFUNCTION()
		void SetAbsolutelyInvincible();

	// F7:���S����
	UFUNCTION()
		void SetDeath();
private:

	// �L�����N�^�[�̉�]
	void RotateCharacter(float _nowInput_X);

	// ���S�����Ăяo��
	void CallControllerFunc_DestroyPlayer();

	// ���G����
	void SetInvincibility() {IsInvincibility = false;}

	// �U���Ɋւ���t���O�����ׂă��Z�b�g
	void ResetAllAttackFlags();

	// �A�j���[�V�����Đ�
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

protected:

	// �Q�[���J�n���ɌĂяo����鏈��
	virtual void BeginPlay() override;

	// ���t���[���Ăяo����鏈��
	virtual void Tick(float DeltaTime) override;

	// ���͂̃Z�b�g�A�b�v
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �f�o�b�O�L�[�Ɋ֐������蓖�Ă�
	void SetupDebugEventBindings();

	//------------------�v���C���[�̃Z�b�g�A�b�v----------------------------------------------------------------------
	// �v���C���[�̃f�[�^��������
	virtual void InitPlayerData();

	// �v���C���[�̃X�e�[�^�X������
	void SetupPlayerStatus();

protected:

	// �v���C��[�̃A�N�V�����Ǘ��p
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
		FMainAction MainActionMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float InvincibilityTime_Short;			// �Z�����G����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float InvincibilityTime_Long;			// �������G����

	// �v���C���[�̃A�j���[�V�����Ǘ��p
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		FPlayerAnimation PlayerAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		FAttackSound PlayerSounds;

	// ��_���[�W���̃G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		class UNiagaraSystem* HitEffect;

	// �J�����̗h���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class UCameraShakeBase> ShakePattern;
	//-------------------------------------------------------------------------------------------------------------


	//-----------------�R���|�[�l���g�ϐ�--------------------------------------------------------------------------
	
	// �f�o�b�O�L�[�ݒ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
		class UDebugComponent* DebugComponent;

	// �L�����N�^�[�̓���
	UCharacterMovementComponent* CharacterMovementComp;
	//-------------------------------------------------------------------------------------------------------------

	// ���S�A�j���[�V�����œ|��Ă���̍Đ��X�s�[�h(1�Œʏ�X�s�[�h)
	float DeadAnimRate = 0.01f;			

	// ����ł���Q�[���I�[�o�[�܂ł̎���(�b)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float DeadToGameOverTime = 3.f;		

	// �q�b�g�X�g�b�v�̎���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float HitStopSpeed = 0.03f;

	// �q�b�g�X�g�b�v���鎞��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float HitStopTime = 0.1f;

private:

	// �v���C���[�̃X�e�[�g
	EPlayerState State;

	// �R��U�������ǂ���
	bool IsKicking;

	// �f�o�b�O�p��Ζ��G
	bool AbsolutelyInvincible = false;

	// ���G��Ԃ��ǂ���
	bool IsInvincibility;

	// �t���[���J�E���g�p
	float frames;	

	// �W�����v�̌v�Z
	const float radPerFrame = 3.14f / 30.f;

	// �W�����v�O�̍���
	float JumpBeforePos_Z;	

	// �R���{�p���ł��邩
	bool CanCombo;							

	// ���i�ڂ̃R���{��
	int ComboIndex;

	// ����U�����ꂽ�畐��𗎂Ƃ���
	int32 DefaultDamageLimit;

	// �R���{�̒i��(First,Second,Third�E�E�E)
	TArray<FName> ComboStartSectionNames;	

	// �n���h��
	FTimerHandle TimerHandle;		

	// �Q�[�����[�h�ۑ�
	class ANEOGameMode* pGameMode;

	// �R���g���[���[�ۑ�
	class ANEOPlayerController* PlayerController;
};