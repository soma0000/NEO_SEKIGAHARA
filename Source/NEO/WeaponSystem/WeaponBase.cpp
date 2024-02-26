// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "NEO/CharacterSystem/ActionAssistComponent.h"

#define JUMP_HEIGHT (PreviousHeight + InitialVelocity * Frames - 0.5f * Gravity * Frames * Frames)
#define JUMP_VELOCITY (InitialVelocity - Gravity * Frames) 
#define DROP_WEAPON_ANGLE (90.0)


// Sets default values
AWeaponBase::AWeaponBase()
	: WeaponState(EWeaponState::PickUp)
	, Damage(0)
	, Frames(0.f)
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

	if (WeaponState == EWeaponState::Fall)
	{
		BlowsAway(DeltaTime);
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
void AWeaponBase::SetupWeaponMesh(TObjectPtr<UStaticMeshComponent>& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// ����̃R���|�[�l���g���쐬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(PublicName);

	if (!MeshComp || !WeaponAssetPath) { return; }

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

/*
 * �֐����@�@�@�@�FSetupWeaponMesh()
 * �������e�@�@�@�F�v���C���[�̃��b�V�����Z�b�g�A�b�v(������SkeletalMesh�̏ꍇ)
 * �����P�@�@�@�@�FUSkeletalMeshComponent*& MeshComp�E�E�E���b�V���R���|�[�l���g
 * �����Q�@�@�@�@�FTCHAR* WeaponAssetPath �E�E�E�E�E�E�E�E����̃A�Z�b�g�̃p�X
 * �����R�@�@�@�@�FFName PublicName �E�E�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::SetupWeaponMesh(TObjectPtr<USkeletalMeshComponent>& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// ����̃R���|�[�l���g���쐬
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (!MeshComp || !WeaponAssetPath) { return; }

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
	if (WeaponState != EWeaponState::PickUp) { return; }

	// ������Ă����Ԃ�
	WeaponState = EWeaponState::Held;

	// �I�[�i�[��ݒ�
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

	// ���ł����Ԃ�
	WeaponState = EWeaponState::Fall;

	// �I�[�i�[���O��
	pOwner = nullptr;

	// �W�����v�O�̍����擾
	PreviousHeight = GetActorLocation().Z;
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
	
	return GetHitResults(_outHitResults,Start, End);
}


/*
 * �֐����@�@�@�@�FGetHitResults()
 * �������e�@�@�@�F�U���������������ǂ�������
 * �����P		 �FTArray<FHitResult>& _outHitResults�E�E�E���������I�u�W�F�N�g
 * �����Q�@�@�@�@�FFVector _start �E�E�E�E�E�E�E�E �E�E�E�E�����蔻����Ƃ�͈�(�J�n�n�_)
 * �����R		 �FFVector _end�E�E�E�E�E�E�E�E�E�E�E�E�E�E�����蔻����Ƃ�͈�(�I���n�_)
 * �߂�l�@�@�@�@�F�����������ǂ���
 */
bool AWeaponBase::GetHitResults(TArray<FHitResult>& _outHitResults,FVector _start, FVector _end)
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
void AWeaponBase::BlowsAway(float DeltaTime)
{
	// ���݈ʒu�擾
	FVector NowPos = GetActorLocation();

	// �W�����v�O�̍�������ʒu�X�V
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, JUMP_HEIGHT));
	SetActorLocation(nextPos);

	// �t���[���X�V
	Frames += DeltaTime * 20.f;

	// ���݂̉�]�擾
	FRotator NowRotation = GetActorRotation();

	// ��ɉ�]
	SetActorRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw, NowRotation.Roll + 10.0));

	// ���n����
	if (JUMP_VELOCITY > 0.f) { return; }
	if (IsLanding()) { HandleLanding(); }
}


/*
 * �֐����@�@�@�@�FIsLanding()
 * �������e�@�@�@�F���n����
 * �߂�l�@�@�@�@�F���n���Ă��邩
 */
bool AWeaponBase::IsLanding()
{
	// �n�_�ƏI�_���w��
	FVector start = GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z - RayLength);

	// ���g������
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// ����
	FHitResult OutHit;
	GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);

	// �n�ʂɃq�b�g�����ꍇtrue
	AActor* HitActor = OutHit.GetActor();

	bool IsLand = HitActor && HitActor->ActorHasTag("Land");

	return IsLand;
}


/*
 * �֐����@�@�@�@�FHandleLanding()
 * �������e�@�@�@�F���n����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::HandleLanding()
{
	// �t���[�����Z�b�g
	Frames = 0.f;

	// �n�ʂɎh����悤�Ɉʒu�Ɗp�x��␳
	AdjustLandingPosition();
	AdjustLandingAngle();

	// �������Đ�
	if (DropWeaponSoundObj) { ActionAssistComp->PlaySound(DropWeaponSoundObj); }

	// �G�t�F�N�g��������悤�ɂ���
	if (AuraEffect) { AuraEffect->SetVisibility(true); }

	// ������Ă��Ȃ���Ԃɂ���
	WeaponState = EWeaponState::PickUp;
}


/*
 * �֐����@�@�@�@�FAdjustLandingPosition()
 * �������e�@�@�@�F���n���Ɉʒu��␳
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AdjustLandingPosition()
{
	FVector NowPos = GetActorLocation();

	SetActorLocation(FVector(NowPos.X, NowPos.Y, NowPos.Z));
}


/*
 * �֐����@�@�@�@�FAdjustLandingPosition()
 * �������e�@�@�@�F���n���Ƀ����_���Ɋp�x��␳
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AdjustLandingAngle()
{
	// �p�x�̗v�f���擾
	int AnglesElement = CorrectDropAngles.Num();

	// ���݂̊p�x�擾
	FRotator NewAngle = FRotator::ZeroRotator;

	// �p�ӂ��ꂽ�p�x�̒����烉���_���Ō���
	if (WeaponType == EWeaponType::Gun)
	{
		NewAngle.Pitch = (AnglesElement > 0) ? (CorrectDropAngles[FMath::Rand() % AnglesElement]) : (DROP_WEAPON_ANGLE - DROP_WEAPON_ANGLE);
		NewAngle.Yaw = (FMath::RandBool()) ? (DROP_WEAPON_ANGLE) : (-DROP_WEAPON_ANGLE);
	}
	else
	{
		NewAngle.Roll = (AnglesElement > 0) ? (CorrectDropAngles[FMath::Rand() % AnglesElement]) : (DROP_WEAPON_ANGLE - DROP_WEAPON_ANGLE);
		NewAngle.Yaw = (FMath::RandBool()) ? (DROP_WEAPON_ANGLE - DROP_WEAPON_ANGLE) : (DROP_WEAPON_ANGLE + DROP_WEAPON_ANGLE);
	}

	SetActorRotation(NewAngle);
}