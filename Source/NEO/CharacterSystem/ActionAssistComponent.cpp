// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionAssistComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "NiagaraFunctionLibrary.h"		
#include "PlayerSystem/NEOPlayerController.h"

#define DIRECTION_Y (90.f)

// Sets default values for this component's properties
UActionAssistComponent::UActionAssistComponent()
	: bUseCorrectAttackAngle(true)
	, bUseHitStop(true)
	, bUseHitEffect(true)
	, bUseFaceCamera(true)
{
	// Tick()�𖈃t���[���Ă΂Ȃ��悤�ɂ���
	PrimaryComponentTick.bCanEverTick = false;

	RayLength_CorrectAngle = 300.f;
	MaxCorrectAngle = 10.f;
}


// Called when the game starts
void UActionAssistComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UActionAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


/*
 * �֐����@�@�@�@�FCorrectAttackAngle()
 * �������e�@�@�@�F�p�x��␳���čU���𓖂���₷������(������ɂ���G)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::CorrectAttackAngle()
{
	// �@�\�̃I���E�I�t
	if (!bUseCorrectAttackAngle) { return; }

	// �I�[�i�[�擾
	AActor* Owner = GetOwner();

	//�@�O���ɂ���G�l�~�[�擾
	AActor* InFrontOfEnemy = GetFrontActor();

	if (Owner && InFrontOfEnemy)
	{
		// �v���C���[�ƓG�̕������擾����
		FVector direction = InFrontOfEnemy->GetActorLocation() - Owner->GetActorLocation();
		direction.Normalize();

		// �v���C���[�̌������擾����
		FRotator CurrentRotation = Owner->GetActorRotation();

		// �␳�p�x�擾
		float CorrectAngle = direction.Rotation().Yaw;

		// �ő�p�x�𒴂�����
		if (FMath::Abs(CorrectAngle) <= MaxCorrectAngle)
		{
			CorrectAngle = (CorrectAngle < 0) ? (-MaxCorrectAngle) : (MaxCorrectAngle);
		}

		// ���L�҂̌�����ύX����
		FRotator NewRotation(CurrentRotation.Pitch, CorrectAngle, CurrentRotation.Roll);
		Owner->SetActorRotation(NewRotation);
	}
}


/*
 * �֐����@�@�@�@�FHitStop()
 * �����P�@�@�@�@�Ffloat _speedDuringHitStop�E�E�E�~�܂��Ă���Ƃ��̃X�s�[�h(1.f�Œʏ�̑���)
 * �����Q�@�@�@�@�Ffloat _stopTime�E�E�E�~�߂鎞��
 * �������e�@�@�@�F�q�b�g�X�g�b�v���N����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::HitStop(float _speedDuringHitStop, float _stopTime)
{
	// �@�\�̃I���E�I�t
	if (!bUseHitStop) { return; }

	// �L�����N�^�[�̏��擾
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	// �L�����N�^�[���擾�ł��Ȃ������烊�^�[��
	if (!Character) { return; }

	// �L�����N�^�[�̃��b�V���擾
	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();

	// �L�����N�^�[�̃��b�V�����擾�ł��Ȃ������烊�^�[��
	if (!CharacterMesh) { return; }

	// HitStop���J�n
	CharacterMesh->GlobalAnimRateScale = _speedDuringHitStop;

	// HitStop���~
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_HitStop, this, &UActionAssistComponent::EndHitStop, _stopTime, false);
}


/*
 * �֐����@�@�@�@�FSpawnEffect()
 * �����P�@�@�@�@�FUNiagaraSystem* _hitParticle�E�E�E�o��������G�t�F�N�g
 * �����Q�@�@�@�@�FFVector _spawnPos�E�E�E�E�E�E�E�E�E�o��������ꏊ
 * �������e�@�@�@�F�G�t�F�N�g���o������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::SpawnEffect(UNiagaraSystem* _hitParticle, FVector _spawnPos)
{
	// �@�\�̃I���E�I�t
	if (!bUseHitEffect) { return; }

	// �G�t�F�N�g�̏�񂪎擾�ł��Ȃ���΃��^�[��
	if (!_hitParticle) { return; }

	//�G�t�F�N�g���o��
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner()->GetWorld(), _hitParticle, _spawnPos);
}

/*
 * �֐����@�@�@�@�FCameraShake()
 * �����P�@�@�@�@�FTSubclassOf<UCameraShakeBase> _shakePattern�E�E�E�h�炷�p�^�[��
 * �����Q�@�@�@�@�Ffloat _scale�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E����
 * �������e�@�@�@�F�J�����V�F�C�N
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::CameraShake(TSubclassOf<UCameraShakeBase> _shakePattern, float _scale /*= 1.f*/)
{
	// �v���C���[�擾
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) { return; }

	// �}�l�[�W���[�擾
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!CameraManager) { return; }

	// �J�����V�F�C�N�J�n
	CameraManager->StartCameraShake(_shakePattern, _scale);
}


/*
 * �֐����@�@�@�@�FPlaySound()
 * �������e�@�@�@�F�����Đ�
 * �����P�@�@�@�@�FUSoundBase* _sound_Obj�E�E�E�����̃A�Z�b�g
 * �����Q�@�@�@�@�F�������J�n���鎞��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::PlaySound(USoundBase* _sound_Obj, float _startTime /*= 0*/)
{
	if (_sound_Obj)
	{
		// �Đ�(7�Ԗڂ̈������ŃI�[�i�[�A�N�^�[��������)
		UGameplayStatics::PlaySound2D(GetWorld(), _sound_Obj, 1.0f, 1.0f, _startTime, nullptr, GetOwner());
	}
}


/*
 * �֐����@�@�@�@�FGetFrontActor()
 * �������e�@�@�@�F�G��������ɂ��邩����
 * �߂�l�@�@�@�@�F�������G�̏���Ԃ�
 */
AActor* UActionAssistComponent::GetFrontActor()
{
	// ���L�҂̏��擾
	AActor* pOwner = GetOwner();

	if (pOwner)
	{
		// ���C���΂�
		// ��΂������w��
		float Rotation_Z = pOwner->GetActorRotation().Yaw;
		float LineDirection = (Rotation_Z > 0) ? (RayLength_CorrectAngle) : (-RayLength_CorrectAngle);

		// �n�_
		FVector start = pOwner->GetActorLocation();

		// �I�_
		FVector end = FVector(start.X, start.Y + LineDirection, start.Z);

		// ���g������
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(pOwner);

		// ���������I�u�W�F�N�g�i�[�p
		FHitResult OutHit;

		// �q�b�g�����ꍇtrue
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_WorldStatic, CollisionParams);
		//DrawDebugLine(GetWorld(), start, end, FColor::Green, true, 1.0f);

		if (isHit)
		{
			AActor* HitEnemy = OutHit.GetActor();

			// "Enemy"�^�O�������Ă���Actor�݂̂�Ԃ�
			if (HitEnemy->ActorHasTag("Enemy"))
			{
				return HitEnemy;
			}
		}
	}

	return nullptr;
}


/*
 * �֐����@�@�@�@�FEndHitStop()
 * �������e�@�@�@�F�q�b�g�X�g�b�v�I��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::EndHitStop()
{
	// �L�����N�^�[�擾
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AttackAssitComponent: Character is nullptr"));
		return;
	}

	//HitStop���~
	Character->GetMesh()->GlobalAnimRateScale = 1.f;
}


/*
 * �֐����@�@�@�@�FOwnerParallelToCamera()
 * ����			 �Fbool _lookRight�E�E�E���݉E�������Ă��邩
 * �������e�@�@�@�F�I�[�i�[���J�����Ɍ�����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UActionAssistComponent::OwnerParallelToCamera(bool _lookRight)
{
	// �v���C���[�R���g���[���[�擾
	ANEOPlayerController* PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));;
	if (!PlayerController) { return; }

	// �J�����̊p�x���擾
	const FRotator CameraRotation = PlayerController->GetNowCameraRotation();

	// �J�����̃s�b�`�ƃ��[
	double Camera_Pitch = CameraRotation.Pitch;
	double Camera_Yaw = CameraRotation.Yaw;

	// �V�����p�x�擾
	const double NewAngle_Pitch = GetAngle_Pitch(_lookRight, Camera_Pitch);
	const double NewAngle_Yaw = GetAngle_Yaw(_lookRight, Camera_Yaw);

	FRotator NewAngle = FRotator(0.0, NewAngle_Yaw, NewAngle_Pitch);

	// ��]
	GetOwner()->SetActorRotation(NewAngle);
}


/*
 * �֐���		�FGetAngle_Yaw()
 * �����P		�Fbool _lookRight�E�E�E���݉E�������Ă��邩
 * �����Q		�Fdouble _camera_Yaw�E�E�E�J�����̌���
 * �������e		�F���̊p�x�擾
 * �߂�l		�F���̊p�x
 */
double UActionAssistComponent::GetAngle_Yaw(bool _lookRight, double _camera_Yaw)
{
	// �V�����p�x����
	double NewAngle = (!_lookRight)?(_camera_Yaw + DIRECTION_Y):(_camera_Yaw - DIRECTION_Y);

	return NewAngle;
}


/*
 * �֐���		�FGetAngle_Pitch()
 * �����P		�Fbool _lookRight�E�E�E���݉E�������Ă��邩
 * �����Q		�Fdouble _camera_Pitch�E�E�E�J�����̌���
 * �������e		�F���̊p�x�擾
 * �߂�l		�F���̊p�x
 */
double UActionAssistComponent::GetAngle_Pitch(bool _lookRight, double _camera_Pitch)
{
	// �@�\�̃I���I�t
	if (!bUseFaceCamera) { return 0.0; }

	// �V�����p�x����
	double NewAngle = (!_lookRight)?(_camera_Pitch):(-_camera_Pitch);

	return NewAngle;
}


// �ǂƂ̃��C�L���X�g���s���֐�
bool UActionAssistComponent::WallCheck(float _lineLength)
{
	// �I�[�i�[�擾
	AActor* pOwner = GetOwner();

	if (!pOwner) { return false; }

	// ���C�L���X�g�����s����ۂ̃p�����[�^��ݒ肷��
	// ���C�L���X�g�̊J�n�ʒu�̓L�����N�^�[�̌��݈ʒu
	float Rotation_Z = pOwner->GetActorRotation().Yaw;
	float LineLength = (Rotation_Z > 0) ? (_lineLength) : (-_lineLength);

	// �n�_
	FVector start = pOwner->GetActorLocation();
	// �I�_
	FVector end = FVector(start.X, start.Y + LineLength, start.Z);

	FCollisionQueryParams CollisionParams;
	// �I�[�i�[���g�͏��O
	CollisionParams.AddIgnoredActor(pOwner);

	FHitResult HitResult;

	// ���C�g���[�X���s��
	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_WorldStatic, CollisionParams);


	return IsHit;
}
