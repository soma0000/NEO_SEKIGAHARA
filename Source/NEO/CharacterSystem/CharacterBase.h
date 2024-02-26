
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

	// �U������
	virtual void Attack() { return; }

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
	virtual void Jump(float _deltaTime) { return; }

	// �q�b�g�X�g�b�v����
	void HitStop(float _speedDuringHitStop, float _stopTime);

	// �X�e�[�^�X�ݒ�
	void InitStatus(int32 _hp, float _moveSpeed, float _jumpHeight, float _comboDamageFactor);

	// �X�e�[�^�X�擾
	FCharacterStatus GetStatus()const { return Status; }

	// ���炩�̃A�j���[�V�������Đ������ǂ���
	bool IsAnimationPlaying()const;

	// �w�肵���p�X�̃A�j���[�V�����A�Z�b�g��Ԃ�
	UAnimMontage* GetAnimationAsset(TCHAR* _animAssetPath);

private:

	// �q�b�g�X�g�b�v�I������
	void EndHitStop();

protected:

	//-----------------�R���|�[�l���g�ϐ�--------------------------------------

	// �A�N�V�����̃A�V�X�g�p�R���|�[�l���g
	UPROPERTY(EditAnywhere, Category = "Action Assist")
		TObjectPtr<class UActionAssistComponent> ActionAssistComp;

	//--------------------------------------------------------------------------

private:

	// �L�����N�^�[�̊�{�X�e�[�^�X
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FCharacterStatus Status;

	// �^�C�}�[�n���h��
	FTimerHandle TimerHandle_HitStop;
};