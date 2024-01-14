
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "NEO/WeaponSystem/WeaponBase.h"

#include "CharacterBase.generated.h"



//-----------------�L�����N�^�[�̃X�e�[�^�X-------------------------------------------------
USTRUCT(BlueprintType)
struct FCharacterStatus
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 HP;								// �q�b�g�|�C���g

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MoveSpeed;						// �ړ����x

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float JumpHeight;						// �W�����v��

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ComboDamageFactor;				// �R���{���̃_���[�W�{��


	// �������֐�
	FCharacterStatus InitParam(int32 _hp,float _moveSpeed, float _jumpHeight, float _comboDamageFactor)
	{
		FCharacterStatus DefaultStatus;

		DefaultStatus.HP = _hp;
		DefaultStatus.MoveSpeed = _moveSpeed;
		DefaultStatus.JumpHeight = _jumpHeight;
		DefaultStatus.ComboDamageFactor = _comboDamageFactor;
		return DefaultStatus;
	}
};
//----------------------------------------------------------------------------------------

UCLASS()
class NEO_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �U���̓����蔻��𔭐�������
	void SetCollision();

	// �����Ă��镐��̎�ގ擾
	EWeaponType GetWeaponType()const { return WeaponType; }

	// ��_���[�W����
	virtual void TakedDamage(float _damge, bool _bLastAttack = false) { return; }

	// �O������ɂȂɂ����邩�`�F�b�N
	bool WallCheck(float _lineLength);

protected:

	// �ړ�����
	virtual void Move() { return; }
	virtual void Move(const FInputActionValue& _value) { return; }

	// �W�����v����
	virtual void Jump() { return; }

	// �U���������������̏���
	virtual void Attack_Sword(TTuple<TArray<AActor*>,TArray<FVector>> HitActorAndLocation) { return; }	// ���̏���
	virtual void Attack_Lance(TTuple<TArray<AActor*>,TArray<FVector>> HitActorAndLocation) { return; }	// ���̏���	
	virtual void Attack_Gun();																			// �e�̏���

	// ������擾
	bool AttachWeapon(AWeaponBase* _newWeapon, FName _socketName = "None", bool _bDestroy = true);

	// ����𗣂�
	void DetachWeapon(bool _bDestroy = true);

	// ����������ւ���
	bool ChangeWeapon(TSubclassOf<AWeaponBase> _newWeaponClass, FName _socketName = "None",bool _bDestroy = true);

	// �q�b�g�X�g�b�v����
	void HitStop(float _speedDuringHitStop, float _stopTime);

	// ����擾
	AWeaponBase* GetWeapon()const { return Weapon; }

	// �X�e�[�^�X�ݒ�
	void InitStatus(int32 _hp, float _moveSpeed, float _jumpHeight, float _comboDamageFactor);

	// �X�e�[�^�X�擾
	FCharacterStatus GetStatus()const { return Status; }

	// ����̍U���͎擾
	int32 GetWeaponDamage()const { return Weapon->GetDamage(); }

	// �Q�[���J�n���ɕ�����X�|�[��
	void SpawnWeapon_BeginPlay();

	// ���炩�̃A�j���[�V�������Đ������ǂ���
	bool IsAnimationPlaying()const;

	// �w�肵���p�X�̃A�j���[�V�����A�Z�b�g��Ԃ�
	UAnimMontage* GetAnimationAsset(TCHAR* _animAssetPath);

private:

	// ���������I�u�W�F�N�g��Actor�ɕϊ����ăq�b�g�����ʒu���Ԃ�
	TTuple<TArray<AActor*>,TArray<FVector>> HitObjToActor(TArray<FHitResult> _hitResults);

	// �q�b�g�X�g�b�v�I������
	void EndHitStop();

	// �A�j���[�V�����C���X�^���X�ɕ�������Z�b�g
	void SetWeaponType_AnimInstance();

protected:

	//-----------------�R���|�[�l���g�ϐ�--------------------------------------

	// �A�N�V�����̃A�V�X�g�p�R���|�[�l���g
	UPROPERTY(EditAnywhere, Category = "Action Assist")
		TObjectPtr<class UActionAssistComponent> ActionAssistComp;

	//--------------------------------------------------------------------------

	// ����̎w��
	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<AWeaponBase> WeaponClass;

	// ����̃\�P�b�g�̖��O
	UPROPERTY(EditAnywhere, Category = "Weapon")
		TArray<FName> SocketNames;

private:

	// �Q�[���J�n���ɕ�����������邩�ǂ���
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		bool bIsSpawnWeapon_BeginPlay;

	// �L�����N�^�[�̊�{�X�e�[�^�X
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FCharacterStatus Status;

	// ����N���X�̃|�C���^
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AWeaponBase> Weapon;

	// ���ݏ������Ă��镐��𔻕ʂ���Enum
	EWeaponType WeaponType;

	// �^�C�}�[�n���h��
	FTimerHandle TimerHandle_HitStop;
};