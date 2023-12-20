// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NEO/WeaponSystem/WeaponBase.h"
#include "CharacterAnimInstance.generated.h"

//-----------------����̎��-------------------------------------
UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	State_Idle   UMETA(DisplayName = "Idle"),
	State_Move   UMETA(DisplayName = "Move"),
	State_Jump   UMETA(DisplayName = "Jump")
};
//----------------------------------------------------------------

UCLASS()
class NEO_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	// �R���X�g���N�^
	UCharacterAnimInstance();

	// �J�n������
	virtual void NativeBeginPlay() override;

	// ���t���[������(Tick) 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// ����̎�ނ�o�^
	void SetWeaponType(EWeaponType _weaponType = EWeaponType::WeaponType_None) { WeaponType = _weaponType; }

	// �L�����N�^�[�̏�Ԃ��Z�b�g
	void SetCharacterState(ECharacterState _charaState = ECharacterState::State_Idle) { State = _charaState; }

protected:

	// �I�[�i�[���L�����N�^�[�N���X�Ŏ擾
	ACharacter* OwnerCharacter;

	// �I�[�i�[��MovementComponent�ۑ��p
	class UCharacterMovementComponent* CharacterMovementComp;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Owner Status")
		FVector Velocity;

	//���s���x
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		float GroundSpeed;

	// �����Ă��邩
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		bool ShouldMove;

	// �󒆂ɂ����Ԃ��ǂ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		bool IsFall;

	// ���̕���������Ă��邩
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		EWeaponType WeaponType;

	// �L�����N�^�[�̏��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		ECharacterState State;
};
