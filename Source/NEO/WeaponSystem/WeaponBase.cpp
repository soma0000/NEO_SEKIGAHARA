// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "NEO/CharacterSystem/ActionAssistComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
	: IsPickUp(true)
	, IsFalling(false)
	, Frames(0.f)
	, Damage(0)
	, JumpHeight(150.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// �^�O�ݒ�
	Tags.Add("Weapon");

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	AuraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AuraEffect"));

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFalling)
	{
		BlowsAway();
	}
}

/*
 * �֐����@�@�@�@�FSetupWeaponMesh()
 * �������e�@�@�@�F�v���C���[�̃��b�V�����Z�b�g�A�b�v
 * �����P�@�@�@�@�FUStaticMeshComponent*& MeshComp�E�E�E���b�V���R���|�[�l���g
 * �����Q�@�@�@�@�FTCHAR* WeaponAssetPath �E�E�E�E�E�E�E����̃A�Z�b�g�̃p�X
 * �����R�@�@�@�@�FFName PublicName �E�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// ����̃R���|�[�l���g���쐬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// ����̃A�Z�b�g�ݒ�
		ConstructorHelpers::FObjectFinder< UStaticMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetStaticMesh(weaponMesh.Object);

			// ���[�g�ɐݒ�
			RootComponent = MeshComp;
		}

		// �G�t�F�N�g��Ǐ]
		if (AuraEffect)
		{
			AuraEffect->SetupAttachment(RootComponent);
		}
	}
}

/*
 * �֐����@�@�@�@�FSetupWeaponMesh()
 * �������e�@�@�@�F�v���C���[�̃��b�V�����Z�b�g�A�b�v(������SkeletalMesh�̏ꍇ)
 * �����P�@�@�@�@�FUSkeletalMeshComponent*& MeshComp�E�E�E���b�V���R���|�[�l���g
 * �����Q�@�@�@�@�FTCHAR* WeaponAssetPath �E�E�E�E�E�E�E�E����̃A�Z�b�g�̃p�X
 * �����R�@�@�@�@�FFName PublicName �E�E�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// ����̃R���|�[�l���g���쐬
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// ����̃A�Z�b�g�ݒ�
		ConstructorHelpers::FObjectFinder< USkeletalMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetSkeletalMeshAsset(weaponMesh.Object);

			// ���[�g�ɐݒ�
			RootComponent = MeshComp;
		}

		// �G�t�F�N�g��Ǐ]
		if (AuraEffect)
		{
			AuraEffect->SetupAttachment(RootComponent);
		}
	}
}


/*
 * �֐����@�@�@�@�FAttachToCharacter()
 * �������e�@�@�@�F�L�����N�^�[�ɂ�������
 * �����P�@�@�@�@�FACharacter* _owner �E�E�E�E�e�ɂȂ�L�����N�^�[
 * �����Q�@�@�@�@�FFName _socketName�E�E�E�E�E�\�P�b�g�̖��O
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AttachToCharacter(ACharacter* _owner, FName _socketName)
{
	// ���ł���Ƃ��͎��Ȃ�
	if (!GetIsPickUp()) { return; }

	// �擾�o���Ȃ���Ԃ�
	IsPickUp = false;

	// �I�[�i�[�̏��ۑ�
	pOwner = _owner;

	if (AuraEffect)
	{
		// �G�t�F�N�g�������Ȃ�����
		AuraEffect->SetVisibility(false);
	}

	// �L�����N�^�[�ɃA�^�b�`
	AttachToComponent(_owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), _socketName);
}


/*
 * �֐����@�@�@�@�FDetachFromCharacter()
 * �������e�@�@�@�F�L�����N�^�[�ɂ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::DetachFromCharacter()
{
	// �L�����N�^�[����O��
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld,false));

	// ���ł���Ƃ��͏E���Ȃ���Ԃ�
	IsPickUp = true;

	// ������΂��t���O�����Ă�
	IsFalling = true;

	// �I�[�i�[���O��
	pOwner = nullptr;

	// ��ԑO�̏ꏊ�ݒ�
	FlyBeforePos = GetActorLocation();
}


/*
 * �֐����@�@�@�@�FGetHitResults()
 * �������e�@�@�@�F�U���������������ǂ�������(�J�n�ƏI�������Ȃ�)
 * �����P�@�@�@�@�FTArray<FHitResult>& _outHitResults�E�E�E���������I�u�W�F�N�g
 * �߂�l�@�@�@�@�F�����������ǂ���
 */
bool AWeaponBase::GetHitResults(TArray<FHitResult>& _outHitResults)
{
	// �����蔻������͈�
	FVector Start = WeaponCollision->GetComponentLocation();
	FVector End = Start;
	
	return GetHitResults(Start, End, _outHitResults);
}


/*
 * �֐����@�@�@�@�FGetHitResults()
 * �������e�@�@�@�F�U���������������ǂ�������
 * �����P�@�@�@�@�FFVector _start �E�E�E�E�E�E�E�E �E�E�E�E�����蔻����Ƃ�͈�(�J�n�n�_)
 * �����Q		 �FFVector _end�E�E�E�E�E�E�E�E�E�E�E�E�E�E�����蔻����Ƃ�͈�(�I���n�_)
 * �����R		 �FTArray<FHitResult>& _outHitResults�E�E�E���������I�u�W�F�N�g
 * �߂�l�@�@�@�@�F�����������ǂ���
 */
bool AWeaponBase::GetHitResults(FVector _start, FVector _end, TArray<FHitResult>& _outHitResults)
{
	// ����{�̂ƃI�[�i�[�ɂ͓�����Ȃ��悤�ɂ���
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(pOwner);

	// ���������������擾
	FQuat Rot = WeaponCollision->GetComponentQuat();
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(WeaponCollision->GetScaledCapsuleRadius(), WeaponCollision->GetScaledCapsuleHalfHeight());

	// �������Ă��邩�m�F
	bool IsHit = GetWorld()->SweepMultiByChannel(_outHitResults, _start, _end, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionShape, CollisionParams);

	return IsHit;
}


/*
 * �֐����@�@�@�@�FBlowsAway()
 * �������e�@�@�@�F���킪�����Ēn�ʂɎh����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::BlowsAway()
{
	// ���݈ʒu
	FVector NowPos = GetActorLocation();

	// Sin�ō����X�V
	float SinValue = JumpHeight * FMath::Sin(RadPerFrame * Frames);

	// �W�����v�O�̍�������ʒu�X�V
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, SinValue + FlyBeforePos.Z));
	SetActorLocation(nextPos);

	// �t���[���X�V
	Frames += 1.f;

	// ���݂̉�]�擾
	FRotator NowRotation = GetActorRotation();

	// ��ɉ�]
	SetActorRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw, NowRotation.Roll + 10.0));

	// ���̈ʒu���Ⴍ�Ȃ�����I��
	if (NowPos.Z < FlyBeforePos.Z)
	{
		// ������Ă��Ȃ���Ԃɂ���
		IsPickUp = true;

		// ���ł����Ԃ̃t���O�����낷
		IsFalling = false;

		// �t���[�����Z�b�g
		Frames = 0.f;

		// �������Đ�
		if (DropWeaponSoundObj)
		{
			// �a��SE�Đ�
			ActionAssistComp->PlaySound(DropWeaponSoundObj);
		}

		// �n�ʂɎh����悤�Ɉʒu�Ɗp�x��␳
		SetActorLocation(FlyBeforePos + DropLocation);
		SetActorRotation(DropAngle);

		if (AuraEffect)
		{
			// �G�t�F�N�g��������悤�ɂ���
			AuraEffect->SetVisibility(true);
		}
	}
}


