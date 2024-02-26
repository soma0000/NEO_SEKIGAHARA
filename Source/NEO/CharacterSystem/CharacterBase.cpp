// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "NEO/WeaponSystem/Gun.h"
#include "NEO/AnimationSystem/CharacterAnimInstance.h"
#include "ActionAssistComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


/*
 * �֐����@�@�@�@�FHitStop()
 * �������e�@�@�@�F�q�b�g�X�g�b�v
 * �����P�@�@�@�@�Ffloat _speedDuringHitStop�E�E�E�q�b�g�X�g�b�v���̃L�����N�^�[�̑��x
 * �����Q�@�@�@�@�Ffloat _stopTime�E�E�E�E�E�E�E�E�q�b�g�X�g�b�v���|���鎞��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::HitStop(float _speedDuringHitStop, float _stopTime)
{
	// �L�����N�^�[�̃��b�V���擾
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	// �L�����N�^�[�̃��b�V�����擾�ł��Ȃ������烊�^�[��
	if (!CharacterMesh) { return; }

	// HitStop���J�n
	CharacterMesh->GlobalAnimRateScale = _speedDuringHitStop;

	// HitStop���~
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_HitStop, this, &ACharacterBase::EndHitStop, _stopTime, false);
}


// �O������ɂȂɂ����邩�`�F�b�N
bool ACharacterBase::WallCheck(float _lineLength)
{
	if (!this) { return false; }

	// ���C�L���X�g�����s����ۂ̃p�����[�^��ݒ肷��
	// ���C�L���X�g�̊J�n�ʒu�̓L�����N�^�[�̌��݈ʒu
	float Rotation_Z = GetActorRotation().Yaw;
	float LineLength = (Rotation_Z > 0) ? (_lineLength) : (-_lineLength);

	// �n�_
	FVector start = GetActorLocation();
	// �I�_
	FVector end = FVector(start.X, start.Y + LineLength, start.Z);

	FCollisionQueryParams CollisionParams;
	// �I�[�i�[���g�͏��O
	CollisionParams.AddIgnoredActor(this);

	FHitResult HitResult;

	// ���C�g���[�X���s��
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_WorldStatic, CollisionParams);


	return IsHit;
}

/*
 * �֐����@�@�@�@�FInitCharaStatus()
 * �������e�@�@�@�F�X�e�[�^�X������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::InitStatus(int32 _hp, float _moveSpeed, float _jumpHeight, float _comboDamageFactor)
{
	// �X�e�[�^�X������
	Status = Status.InitParam(_hp, _moveSpeed, _jumpHeight, _comboDamageFactor);
}


/*
 * �֐����@�@�@�@�FEndHitStop()
 * �������e�@�@�@�F�q�b�g�X�g�b�v�I��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::EndHitStop()
{
	//HitStop���~
	GetMesh()->GlobalAnimRateScale = 1.f;
}


/*
 * �֐����@�@�@�@�FGetAnimationAsset()
 * �������e�@�@�@�F���炩�̃A�j���[�V�������Đ������ǂ���
 * �߂�l�@�@�@�@�F�Đ����Ȃ�true
 */
bool ACharacterBase::IsAnimationPlaying()const
{
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	UAnimMontage* PlayingAnimation = AnimInstance->GetCurrentActiveMontage();

	// �C���X�^���X��������Ȃ������ꍇ��false
	bool IsPlaying = (!PlayingAnimation)?(false):(true);

	return IsPlaying;
}


/*
 * �֐����@�@�@�@�FGetAnimationAsset()
 * �������e�@�@�@�F�A�j���[�V�����A�Z�b�g��Ԃ�
 * �����P�@�@�@�@�FTCHAR* _animAssetPath �E�E�E�A�j���[�V�����A�Z�b�g�̃p�X
 * �߂�l�@�@�@�@�F���������A�j���[�V����
 */
UAnimMontage* ACharacterBase::GetAnimationAsset(TCHAR* _animAssetPath)
{
	// �A�Z�b�g��T���ăZ�b�g
	ConstructorHelpers::FObjectFinder<UAnimMontage> SearchAnimMontage(_animAssetPath);

	// ���̃A�Z�b�g�����݂�����i�[
	if (SearchAnimMontage.Succeeded())
	{
		UAnimMontage* FoundAnimMontage = SearchAnimMontage.Object;

		return FoundAnimMontage;
	}

	return nullptr;
}