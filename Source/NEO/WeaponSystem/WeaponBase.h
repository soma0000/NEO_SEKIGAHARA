// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class UBoxComponent;		// -------------------------------
class UCapsuleComponnet;	// �R���W�����쐬�e���v���[�g�p
class USphereComponent;		// -------------------------------
class ACharacter;
class UNiagaraSystem;

//-----------------����̎��-------------------------------------
UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	WeaponType_Sword   UMETA(DisplayName = "Sword"),
	WeaponType_Lance   UMETA(DisplayName = "Lance"),
	WeaponType_Gun     UMETA(DisplayName = "Gun"),
	WeaponType_None	   UMETA(DisplayName = "None")
};
//----------------------------------------------------------------

//------����̃X�e�[�g�Ǘ��p-----------------
enum class EWeaponState :uint8
{
	PickUp,	// �E������
	Fall,	// ���ł�����
	Held	// ������Ă�����
};
//-------------------------------------------

UCLASS()
class NEO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// �R���X�g���N�^
	AWeaponBase();

public:

	// �v���C���[�̎�ɕt����
	void AttachToCharacter(ACharacter* _owner, FName _socketName);

	// �v���C���[�̎肩��O���
	void DetachFromCharacter();

	// ����̎�ޔ��ʗp
	EWeaponType GetWeaponType()const { return WeaponType; }

	// �擾�ł����Ԃ�
	bool GetIsPickUp()const { return WeaponState == EWeaponState::PickUp; }

	// �U���͎擾
	int32 GetDamage()const { return Damage; }

	// �U���������������ǂ���
	bool GetHitResults(TArray<FHitResult>& _outHitResults);
	bool GetHitResults(TArray<FHitResult>& _outHitResults,FVector _start, FVector _end);

	// �R���W�����擾�p
	class UCapsuleComponent* GetCollisionComponent()const { return WeaponCollision; }

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �����ɂ���ăX�^�e�B�b�N���b�V�����X�P���^�����b�V���̃Z�b�g�A�b�v
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponStaticMesh");
	void SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponSkeletalMesh");

	// �_���[�W�ʐݒ�
	void SetDamageAmount(int32 _damage) { Damage = _damage; }


private:

	// �I�[�i�[�����Ȃ��Ȃ����Ƃ����킪������
	void BlowsAway();


protected:

	// ����̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMesh")
		class UStaticMeshComponent* WeaponStaticMesh;

	// ����̃R���W����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponCollision")
		class UCapsuleComponent* WeaponCollision;

	// �A�N�V�����⏕�̖���������R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAssist")
		class UActionAssistComponent* ActionAssistComp;

	// ���킪�����Ă���Ƃ��̃G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UNiagaraComponent* AuraEffect;

	// ����𗎂Ƃ������̉�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundBase* DropWeaponSoundObj;

	// ���킪������ꏊ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		FVector DropLocation;

	// ���킪������p�x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
		FRotator DropAngle;

	// �I�[�i�[�̏��
	ACharacter* pOwner;

	// ����𔻕ʂ���Enum
	EWeaponType WeaponType;

private:

	// ����̃X�e�[�g
	EWeaponState WeaponState;

	// �t���[���J�E���g�p
	float Frames;

	// �U����
	UPROPERTY(EditAnywhere,Category = "Status",meta = (AllowPrivateAcces = "true"))
		int32 Damage;

	// �W�����v�̍���
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAcces = "true"))
		float JumpHeight;

	// �W�����v�̌v�Z
	const float RadPerFrame = 3.14f / 30.f;

	// ��ԑO�̈ʒu
	FVector FlyBeforePos;

	// �n���h��
	FTimerHandle TimerHandle;


protected:

	// ---------�R���W�����R���|�[�l���g�쐬�e���v���[�g
	/*
	 * �֐����@�@�@�@�FSetupCollisionComponent()
	 * �������e�@�@�@�F����̃R���W�����Z�b�g�A�b�v
	 * �����P�@�@�@�@�FT* CollisionComp�E�E�E�E�E�E�E�E�E�R���W�����R���|�[�l���g(�uT�v�� UBoxComponent,USphereComponent,UCapsuleComponent �̂����ꂩ)
	 * �����Q�@�@�@�@�FFName PublicName�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
	 * �߂�l�@�@�@�@�F�Ȃ�
	 */
		template<class T>
		void SetupCollisionComponent(T*& CollisionComp, FName PublicName = "CollisionComp")
		{
			static_assert(std::is_same<T, UBoxComponent>::value || std::is_same<T, USphereComponent>::value || std::is_same<T, UCapsuleComponent>::value,
				"�uT�v�� UBoxComponent,USphereComponent,UCapsuleComponent �̂����ꂩ ");
	
			// �Ή�����I�u�W�F�N�g����
			CollisionComp = CreateDefaultSubobject<T>(PublicName);
	
			if (CollisionComp)
			{
				// ����̃��b�V���ɒǏ]
				CollisionComp->SetupAttachment(WeaponStaticMesh, "hand_rSocket");
			}
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------------
};
