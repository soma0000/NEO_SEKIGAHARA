// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "NEO/WeaponSystem/Gun.h"
#include "NEO/AnimationSystem/CharacterAnimInstance.h"
#include "ActionAssistComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
	: bIsSpawnWeapon_BeginPlay(true)
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

	// �Q�[���J�n���ɕ�����X�|�[��
	SpawnWeapon_BeginPlay();
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
 * �֐����@�@�@�@�FSpawnWeapon_BeginPlay()
 * �������e�@�@�@�F�Q�[���J�n���ɕ�����o��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::SpawnWeapon_BeginPlay()
{
	// �Q�[���J�n���ɃX�|�[�����Ȃ��ݒ�̎��̓X���[
	if (!bIsSpawnWeapon_BeginPlay) { return; }

	// �������킪�ݒ肳��Ă��邩�m�F(BP����ݒ�\)
	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// �����Spawn
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

		// �v���C���[�ɃA�^�b�`
		if (Weapon)
		{
			// ����̎�ގ擾
			WeaponType = Weapon->GetWeaponType();

			// AnimationInstance�ɕ�����𑗂�
			SetWeaponType_AnimInstance();

			// �\�P�b�g���ݒ肳��Ă��邩����
			if (SocketNames.Num() > 0)
			{
				Weapon->AttachToCharacter(this, SocketNames[0]);
			}
		}
	}
}


/*
 * �֐����@�@�@�@�FAttack_Gun()
 * �������e�@�@�@�F���������I�u�W�F�N�g��Actor�ɕϊ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::Attack_Gun()
{
	// ���킪�Ȃ�������X���[
	if (!Weapon) { return; }

	// �e�̎ˌ�����
	Cast<AGun>(Weapon)->Shoot(50);
}


/*
 * �֐����@�@�@�@�FAttachWeapon()
 * �������e�@�@�@�F����𑕔�����
 * �����P�@�@�@�@�FAWeaponBase* _newWeapon�E�E�E�擾���镐��
 * �����Q		�FFName _socketName �E�E�E������������邽�߂̃\�P�b�g�̖��O
 * �����R�@�@�@�@�Fbool _bDestroy�E�E�E�E�E����������Ă���ꍇ�����Ɠ����ɍ폜���邩
 * �߂�l�@�@�@�@�F����������������
 */
bool ACharacterBase::AttachWeapon(AWeaponBase* _newWeapon, FName _socketName /*= "None"*/, bool _bDestroy /*= true*/)
{
	// �V�������킪���݂��邩�\�P�b�g���ݒ肳��Ă��邩
	if (!_newWeapon || SocketNames.Num() <= 0) { return false; }

	// ���킪�擾�ł����Ԃ��m�F
	if (!(_newWeapon->GetIsPickUp())) { return false; }

	// ���̕���������Ă���Ƃ�
	if (Weapon)
	{
		// ����𗣂�
		DetachWeapon(_bDestroy);
	}

	// ������X�V
	Weapon = _newWeapon;
	WeaponType = Weapon->GetWeaponType();

	// AnimationInstance�ɕ�����𑗂�
	SetWeaponType_AnimInstance();

	// �V�����\�P�b�g���w�肳�ꂽ��X�V����
	const FName NewSocketName = (_socketName != "None") ? (_socketName) : (SocketNames[0]);

	Weapon->AttachToCharacter(this, NewSocketName);

	return true;
}


/*
 * �֐����@�@�@�@�FDetachWeapon()
 * �������e�@�@�@�F���������I�u�W�F�N�g��Actor�ɕϊ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::DetachWeapon(bool _bDestroy /*= true*/)
{
	if (!Weapon) { return; }

	// �����Ă��镐��𗣂����폜����
	(!_bDestroy) ? (Weapon->DetachFromCharacter()) : (Weapon->Destroy());

	// ����������Ă��Ȃ���Ԃ�
	Weapon = nullptr;
	WeaponType = EWeaponType::WeaponType_None;

	// AnimationInstance�ɕ�����𑗂�
	SetWeaponType_AnimInstance();
}


/*
 * �֐����@�@�@�@�FChangeWeapon()
 * �������e�@�@�@�F�V��������𑕔�����
 * �����P�@�@�@�@�FTSubclassOf<AWeaponBase> _newWeaponClass�E�E�E�V��������̃N���X
 * �����Q		�FFName _socketName �E�E�E�E�E�E�E�E�E�E�E�E�E�E�\�P�b�g�̖��O
 * �����R�@�@�@�@�Fbool _bDestroy�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�����ւ���O�̕�����폜���邩�ǂ���
 * �߂�l�@�@�@�@�F����������������
 */
bool ACharacterBase::ChangeWeapon(TSubclassOf<AWeaponBase> _newWeaponClass, FName _socketName /*= "None"*/, bool _bDestroy /*= true*/)
{
	// �V�������킪�ݒ肳��Ă��邩�m�F
	if (!_newWeaponClass) { return false; }

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ����������Ă���ꍇ�藣��
	if (Weapon){ Weapon->DetachFromCharacter(); }

	// �������������邩�̃t���O
	bool bSucces = false;

	// �v���C���[�ɃA�^�b�`
	if (Weapon = GetWorld()->SpawnActor<AWeaponBase>(_newWeaponClass, SpawnParams))
	{
		// ����̎�ގ擾
		WeaponType = Weapon->GetWeaponType();

		Weapon->AttachToCharacter(this, _socketName);

		bSucces =  true;
	}

	return bSucces;
}


/*
 * �֐����@�@�@�@�FHitObjToActor()
 * �������e�@�@�@�F���������I�u�W�F�N�g��Actor�ɕϊ�
 * �����P�@�@�@�@�FTArray<FHitResult> _hitResults�E�E�E�q�b�g�����I�u�W�F�N�g
 * �߂�l�@�@�@�@�F�ϊ�����Actor�Ƃ��̍��W(�^�O�������Ă��Ȃ�Actor�͏Ȃ�)�̃^�v��
 */
TTuple<TArray<AActor*>, TArray<FVector>> ACharacterBase::HitObjToActor(TArray<FHitResult> _hitResults)
{
	// Actor�Ƃ��̍��W�i�[�p
	TArray<AActor*> HitActors;
	TArray<FVector> HitLocations;

	// ���������I�u�W�F�N�g�̐������J��Ԃ�
	for (const FHitResult HitResult : _hitResults)
	{
		// Actor�擾
		AActor* HitActor = HitResult.GetActor();

		if (HitActor->Tags.Num() > 0)
		{
			// �^�O�������Ă���Actor�Ƃ��̍��W�݂̂�ǉ�
			HitActors.Add(HitActor);
			HitLocations.Add(HitResult.Location);
		}
	}

	// �^�v����Ԃ�
	return MakeTuple(HitActors, HitLocations);
}


/*
 * �֐����@�@�@�@�FSetWeaponType_AnimInstance()
 * �������e�@�@�@�FAnimationInstance�ɕ�������Z�b�g
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::SetWeaponType_AnimInstance()
{
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (!AnimInstance) { return; }

	// AnimationInstance�ɕ�����𑗂�
	AnimInstance->SetWeaponType(WeaponType);
}



/*
 * �֐����@�@�@�@�FSetCollision()
 * �������e�@�@�@�F�U���̓����蔻��𔭐�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ACharacterBase::SetCollision()
{
	if (!Weapon) { return; }

	// ���������I�u�W�F�N�g�i�[�p
	TArray<FHitResult> OutHitResults;

	// ����̎�ނɂ���ď�����ς���
	switch (WeaponType)
	{
	case EWeaponType::WeaponType_Sword:						// ���̍U��

		// ���킪�����ɓ������Ă�����
		if (Weapon->GetHitResults(OutHitResults))
		{
			Attack_Sword(HitObjToActor(OutHitResults));
		}
		break;
	case EWeaponType::WeaponType_Lance:						// ���̍U��
														
		// ���킪�����ɓ������Ă�����					
		if (Weapon->GetHitResults(OutHitResults))
		{												
			Attack_Lance(HitObjToActor(OutHitResults));
		}
		break;
	case EWeaponType::WeaponType_Gun:						// �e�̍U��

		Attack_Gun();										
		break;
	case EWeaponType::WeaponType_None:
		break;
	}

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