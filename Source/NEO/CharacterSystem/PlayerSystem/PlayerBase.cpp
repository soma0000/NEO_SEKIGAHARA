// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DebugComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NEOPlayerController.h"
#include "NEO/GameSystem/NEOGameMode.h"
#include "NEO/BackGroundSystem/ObjectBase.h"
#include "NEO/CharacterSystem/CharacterBase.h"
#include "NEO/CharacterSystem/ActionAssistComponent.h"
#include "NEO/WeaponSystem/Gun.h"

// Sets default values
APlayerBase::APlayerBase()
	: State(EPlayerState::Idle)
	, IsKicking(false)
	, IsInvincibility(false)
	, frames(0.f)
	, CanCombo(true)
	, ComboIndex(0)
{
	// Tick()���}�C�t���[���ĂԂ��ǂ����̔���
	PrimaryActorTick.bCanEverTick = true;

	// �v���C���[�̐ݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// �v���C���[�ɉ�]�̐���
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// �^�O�ݒ�
	Tags.Add("Player");

	// Player�̃Z�b�g�A�b�v
	InitPlayerData();
}


// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	// �X�e�[�^�X�ݒ�
	SetupPlayerStatus();

	// �Q�[�����[�h�擾
	pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// �R���g���[���[�擾
	PlayerController = Cast<ANEOPlayerController>(Controller);

	// ���͂̐ݒ�
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MainActionMapping.DefaultMappingContext, 0);
		}
	}
}


// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �W�����v���̂ݏ���
	if (State == EPlayerState::Jump)
	{
		Jump();
	}
}


// ���͂̃o�C���h
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// �ړ�
		EnhancedInputComponent->BindAction(MainActionMapping.MoveAction, ETriggerEvent::Triggered, this, &APlayerBase::Move);
		EnhancedInputComponent->BindAction(MainActionMapping.MoveAction, ETriggerEvent::Completed, this, &APlayerBase::Stop);

		// �W�����v
		EnhancedInputComponent->BindAction(MainActionMapping.JumpAction, ETriggerEvent::Started, this, &APlayerBase::JumpStart);

		// �U���A�N�V����
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction1, ETriggerEvent::Started, this, &APlayerBase::Attack1);
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction2, ETriggerEvent::Started, this, &APlayerBase::Attack2);


		if (!DebugComponent) { return; }
		// �f�o�b�O�L�[�̐ݒ�
		DebugComponent->SetupDebugInputComponent(PlayerInputComponent);
	}

}


/*
 * �֐����@�@�@�@�FSetupDebugEventBindings()
 * �������e�@�@�@�F�f�o�b�O�L�[�Ɋ֐����o�C���h
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetupDebugEventBindings()
{
	if (!DebugComponent) { return; }

	// ESC		�F�Q�[���I��
	// F�P		�F���X�^�[�g�Q�[��
	// F�Q		�F�C���Q�[������X�^�[�g
	// F�R		�F�|�[�Y
	// F�S		�F�o�g���G���A����
	// F�T		�F
	// F�U		�F���G��
	// F�V		�F���S
	// F�W		�F
	// F�X		�F
	// F�P�O	�F
	// F�P�P	�F
	// F�P�Q	�F

	DebugComponent->DebugInputReceived_ESC.AddDynamic(this, &APlayerBase::QuitGame);
	DebugComponent->DebugInputReceived_F1.AddDynamic(this, &APlayerBase::RestartGame);
	DebugComponent->DebugInputReceived_F2.AddDynamic(this, &APlayerBase::StartInGame);
	DebugComponent->DebugInputReceived_F3.AddDynamic(this, &APlayerBase::SetGamePause);	
	DebugComponent->DebugInputReceived_F4.AddDynamic(this, &APlayerBase::ExitBattleArea);
	DebugComponent->DebugInputReceived_F6.AddDynamic(this, &APlayerBase::SetAbsolutelyInvincible);
	DebugComponent->DebugInputReceived_F7.AddDynamic(this, &APlayerBase::SetDeath);
}


/*
 * �֐����@�@�@�@�FSetupPlayerData()
 * �������e�@�@�@�F�v���C���[�̃f�[�^������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::InitPlayerData()
{
	// �f�o�b�O�R���|�[�l���g�쐬�쐬
	DebugComponent = CreateDefaultSubobject<UDebugComponent>(TEXT("Debug"));

	// �f�o�b�O�p�֐��o�C���h
	SetupDebugEventBindings();

	// �R���W�����C�x���g��ݒ�
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerBase::OnOverlap);

	// �X�e�[�^�X������
	InitStatus(2, 500.f, 150.f, 1.f);

	// ��_���[�W���̖��G���Ԑݒ�
	InvincibilityTime_Short = 0.3f;			// �ʏ���
	InvincibilityTime_Long = 0.5f;			// ����𗎂Ƃ��Ƃ�

	// �����ɂ���ă\�P�b�g��ύX
	SocketNames.Add("hand_rSocket_Sword");
	SocketNames.Add("hand_rSocket_Lance");
	SocketNames.Add("hand_rSocket_Gun");

	// �R���{�̖��O�i�[
	ComboStartSectionNames = { "First", "Second", "Third","Fourth" };
}


/*
 * �֐����@�@�@�@�FSetupPlayerStatus()
 * �������e�@�@�@�F�v���C���[�̃X�e�[�^�X�ݒ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetupPlayerStatus()
{
	// �ړ����x�ݒ�
	GetCharacterMovement()->MaxWalkSpeed = GetStatus().MoveSpeed;

	// ����𗎂Ƃ��܂ł̔�_���[�W�񐔐ݒ�
	DefaultDamageLimit = GetStatus().HP;
}


/*
 * �֐����@�@�@�@�FMove()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�ړ�����)
 * �����P�@�@�@�@�FFInputActionValue& Value�E�E�E���͗�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Move(const FInputActionValue& _value)
{
	// ��~�E�ړ��E�W�����v���̂݉�
	if (!(State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Jump)) { return; }

	// 2���œ��͎擾
	FVector2D MovementVector = _value.Get<FVector2D>();

	if (PlayerController)
	{
		// �J�����̊p�x�擾
		const FRotator Rotation = PlayerController->GetNowCameraRotation();

		// ��]�ݒ�
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// �J�����̌����ɉ������ړ������擾(X,Y)
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// �ړ�
		AddMovementInput(RightDirection, MovementVector.X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// �ړ������ɉ�]
		RotateCharacter(MovementVector.Y);
	}

	// �~�܂��Ă��鎞����̂ݕύX
	if (State == EPlayerState::Idle)
	{
		// �ړ�����
		State = EPlayerState::Move;
	}
}


/*
 * �֐����@�@�@�@�FStop()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(��~����)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Stop()
{
	// �ړ����ȊO�̓X���[
	if (State != EPlayerState::Move) { return; }

	// �X�e�[�g���~��
	State = EPlayerState::Idle;
}


/*
 * �֐����@�@�@�@�FJumpStart()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�W�����v�J�n)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::JumpStart()
{
	if (!IsPlayerGrounded()) { return; }

	// ��~���ƈړ����̂݃W�����v��
	if (State == EPlayerState::Idle || State == EPlayerState::Move) { return; }

	// �W�����v���ԃ��Z�b�g
	frames = 0.f;

	// �W�����v�O�̍����擾
	JumpBeforePos_Z = GetActorLocation().Z;

	// �X�e�[�g���W�����v������
	State = EPlayerState::Jump;
}


/*
 * �֐����@�@�@�@�FJump()
 * �������e�@�@�@�F�v���C���[�̃W�����v������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Jump()
{
	// ���݈ʒu
	FVector NowPos = GetActorLocation();

	// Sin�ō����X�V
	float SinValue = GetStatus().JumpHeight * FMath::Sin(radPerFrame * frames);

	// �W�����v�O�̍�������ʒu�X�V
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, SinValue + JumpBeforePos_Z));

	SetActorLocation(nextPos);

	// ���n���� ���~�J�n���画��J�n
	if (frames >= 10.f && (IsPlayerGrounded() || nextPos.Z <= JumpBeforePos_Z))
	{
		State = EPlayerState::Idle;
	}

	// �t���[��+1
	frames += 1.f;
}


/*
 * �֐����@�@�@�@�FIsPlayerGrounded()
 * �������e�@�@�@�F�v���C���[���n�ʂɂ��Ă��邩����
 * �߂�l�@�@�@�@�F�n�ʂɂ��Ă�����true
 */
bool APlayerBase::IsPlayerGrounded() const
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();

	// �ڒn���Ă��邩�@���[�u�����g�R���|�[�l���g���擾�ł��Ȃ��ꍇ�͕K��false
	bool IsGrounded = (!MovementComp) ? (false) : (!(MovementComp->IsFalling()));

	return IsGrounded;
}


// �ڐG�J�n���ɍs������
void APlayerBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �U�����Ɣ�_���[�W���͕�����E���Ȃ���Ԃ�
	if (State == EPlayerState::Attack || State == EPlayerState::Damage || State == EPlayerState::Death) { return; }

	// ���������̂��v���C���[�̎�����������
	if (OtherActor && OtherActor->ActorHasTag("Weapon"))
	{
		// �V��������̏��擾
		AWeaponBase* NewWeapon = Cast<AWeaponBase>(OtherActor);

		// ������E��
		if (AttachWeapon(NewWeapon, SocketNames[int32(NewWeapon->GetWeaponType())], false))
		{
			// ����𗎂Ƃ��܂ł̔�_���[�W�񐔃��Z�b�g
			DefaultDamageLimit = GetStatus().HP;
		}
	}
}


/*
 * �֐����@�@�@�@�FComboAttack()
 * �������e�@�@�@�F�v���C���[�̍U������
 * �����P�@�@�@�@�Fint _attackNum�E�E�E�U���A�j���[�V�����̎�ޔ��ʗp
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::ComboAttack(int _attackNum /*= 0*/)
{
	switch (GetWeaponType())
	{
	case EWeaponType::WeaponType_Sword:
		PlayComboAnimtion_Sword(_attackNum);
		break;
	case EWeaponType::WeaponType_Lance:
		PlayComboAnimtion_Lance(_attackNum);
		break;
	case EWeaponType::WeaponType_Gun:
		PlayComboAnimtion_Gun(_attackNum);
		break;
	case EWeaponType::WeaponType_None:

		break;
	}
}


/*
 * �֐����@�@�@�@�FAttack1()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�U���P��)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Attack1()
{
	if (!(State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Jump || State == EPlayerState::Attack)) { return; }

	// �R���{�U��
	ComboAttack(0);
}

/*
 * �֐����@�@�@�@�FAttack2()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�U���Q��)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Attack2()
{
	if (!(State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Jump || State == EPlayerState::Attack)) { return; }

	// �R���{�U��
	ComboAttack(1);
}


/*
 * �֐����@�@�@�@�FAttack_Sword()
 * �������e�@�@�@�F���������������̏���
 * �����P�@�@�@�@�FTTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation�E�E�E���ɓ�������Actor�Ɠ��������ʒu�̏��̃^�v��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Attack_Sword(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation)
{
	// ���킪�擾�ł��Ȃ��Ƃ��������Ȃ�
	if (!GetWeapon()) { return; }

	// ��������Actor�ƈʒu���擾
	TArray<AActor*> HitActors = HitActorAndLocation.Get<0>();
	TArray<FVector> HitLocations = HitActorAndLocation.Get<1>();

	// �R���{���ŏI�i���ǂ����擾���鎮
	auto GetIsLastAttack = [this]()->bool 
	{
		bool IsLastAttack = GetComboIndex() == ComboStartSectionNames.Num() - 1;

		return IsLastAttack;
	};

	// �q�b�g����Actor�̐������J��Ԃ�
	for (int i = 0; i < HitActors.Num(); ++i)
	{
		// Actor�Ƃ��̍��W�擾
		AActor* HitActor = HitActors[i];
		const FVector HitLocation = HitLocations[i];

		// �I�u�W�F�N�g�ɓ������Ă���ꍇ
		if (HitActor && HitActor->ActorHasTag("Object"))
		{
			// �j��\�I�u�W�F�N�g�̏��擾
			AObjectBase* BreakObj = Cast<AObjectBase>(HitActor);

			// �j��!!
			if (BreakObj)
			{
				BreakObj->ReceiveDamage(GetDamageAmount());
			}
			break;
		}
		// �G�ɓ������Ă���ꍇ
		else if (HitActor && HitActor->ActorHasTag("Enemy"))
		{
			// �q�b�g�����G�擾
			ACharacterBase* HitEnemy = Cast<ACharacterBase>(HitActor);

			// �G�ւ̍U��
			if (HitEnemy)
			{
				// �G�l�~�[�ɍU��
				HitEnemy->TakedDamage(GetDamageAmount(), GetIsLastAttack());
			}

			// �q�b�g�X�g�b�v
			HitStop(HitStopSpeed, HitStopTime);

			// �q�b�g�G�t�F�N�g�\��
			if (HitEffect)
			{
				ActionAssistComp->SpawnEffect(HitEffect, HitLocations[i]);
			}

			// �R���{�̍Ō�ɃJ������h�炷
			if (GetIsLastAttack())
			{
				float ForwardVector_Y = GetActorForwardVector().Y;

				ActionAssistComp->CameraShake(ShakePattern, ForwardVector_Y);
			}

			// ���p�̃q�b�g���Đ�
			ActionAssistComp->PlaySound(PlayerSounds.SwordAttack);
		}
	}
}


/*
 * �֐����@�@�@�@�FAttack_Lance()
 * �������e�@�@�@�F���������������̏���
 * �����P�@�@�@�@�FTTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation�E�E�E���ɓ�������Actor�Ɠ��������ʒu�̏��̃^�v��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Attack_Lance(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation)
{
	// ���킪�擾�ł��Ȃ��Ƃ��������Ȃ�
	if (!GetWeapon()) { return; }

	// ��������Actor�ƈʒu���擾
	TArray<AActor*> HitActors = HitActorAndLocation.Get<0>();
	TArray<FVector> HitLocations = HitActorAndLocation.Get<1>();

	// �R���{���ŏI�i���ǂ����擾���鎮
	auto GetIsLastAttack = [this]()->bool
	{
		bool IsLastAttack = GetComboIndex() == ComboStartSectionNames.Num() - 1;

		return IsLastAttack;
	};

	// �q�b�g����Actor�̐������J��Ԃ�
	for (int i = 0; i < HitActors.Num(); ++i)
	{
		// Actor�Ƃ��̍��W�擾
		AActor* HitActor = HitActors[i];
		const FVector HitLocation = HitLocations[i];

		// �I�u�W�F�N�g�ɓ������Ă���ꍇ
		if (HitActor && HitActor->ActorHasTag("Object"))
		{
			// �j��\�I�u�W�F�N�g�̏��擾
			AObjectBase* BreakObj = Cast<AObjectBase>(HitActor);

			// �j��!!
			if (BreakObj)
			{
				BreakObj->ReceiveDamage(GetDamageAmount());
			}
			break;
		}
		// �G�ɓ������Ă���ꍇ
		else if (HitActor && HitActor->ActorHasTag("Enemy"))
		{
			// �q�b�g�����G�擾
			ACharacterBase* HitEnemy = Cast<ACharacterBase>(HitActor);

			// �G�ւ̍U��
			if (HitEnemy)
			{
				// �G�l�~�[�ɍU��
				HitEnemy->TakedDamage(GetDamageAmount(), GetIsLastAttack());
			}

			// �q�b�g�X�g�b�v
			HitStop(HitStopSpeed, HitStopTime * float(ComboIndex + 1));

			// �q�b�g�G�t�F�N�g�\��
			if (HitEffect)
			{
				ActionAssistComp->SpawnEffect(HitEffect, HitLocations[i]);
			}

			// �R���{�̍Ō�ɃJ������h�炷
			if (GetIsLastAttack())
			{
				float ForwardVector_Y = GetActorForwardVector().Y;

				ActionAssistComp->CameraShake(ShakePattern, ForwardVector_Y);
			}

			// ���p�̃q�b�g���Đ�
			ActionAssistComp->PlaySound(PlayerSounds.LanceAttack);
		}
	}
}


/*
 * �֐����@�@�@�@�FAttack_Gun()
 * �������e�@�@�@�F�e�̍U������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Attack_Gun()
{
	// ���킪�擾�ł��Ȃ��Ƃ��������Ȃ�
	if (!GetWeapon()) { return; }

	if (!IsKicking)
	{
		// �e�̎ˌ�����
		Cast<AGun>(GetWeapon())->Shoot();

		return;
	}

	// �����̈ʒu���擾
	FVector Start = GetMesh()->GetSocketLocation("player_finished1_R_ankle");
	FVector End = Start;

	// ���������I�u�W�F�N�g���i�[����
	TArray<FHitResult> OutHitResults;

	//�������Ă��邩�m�F
	bool IsHit = GetWeapon()->GetHitResults(OutHitResults, Start, End);

	// �������Ă��Ȃ�������X���[
	if (!IsHit) { return; }

	// �ŏ��ɓ��������I�u�W�F�N�g�̂ݎg�p
	FHitResult FirstHitObj = OutHitResults[0];

	// �ŏ��ɓ�������Actor�Ɠ��������ʒu���擾
	AActor* HitActor = FirstHitObj.GetActor();
	FVector HitLocation = FirstHitObj.Location;

	// �I�u�W�F�N�g�ɓ������Ă���ꍇ
	if (HitActor && HitActor->ActorHasTag("Object"))
	{
		// �j��\�I�u�W�F�N�g�̏��擾
		AObjectBase* BreakObj = Cast<AObjectBase>(HitActor);

		// �j��!!
		if (BreakObj)
		{
			BreakObj->ReceiveDamage(GetDamageAmount());
		}
	}
	// �G�ɓ������Ă���ꍇ
	else if (HitActor && HitActor->ActorHasTag("Enemy"))
	{
		// �q�b�g�����G�擾
		ACharacterBase* HitEnemy = Cast<ACharacterBase>(HitActor);

		if (HitEnemy)
		{
			// �G�ւ̍U��
			HitEnemy->TakedDamage(10.f);
		}

		// �q�b�g�X�g�b�v
		HitStop(HitStopSpeed, HitStopTime);

		// �q�b�g�G�t�F�N�g�\��
		if (HitEffect)
		{
			ActionAssistComp->SpawnEffect(HitEffect, HitLocation);
		}

		// �R��p�̃q�b�g���Đ�
		ActionAssistComp->PlaySound(PlayerSounds.Kick);
	}
}


/*
 * �֐����@�@�@�@�FPlayComboAnimtion_Sword()
 * �������e�@�@�@�F���̍U���A�j���[�V����
 * �����P�@�@�@�@�Fint _attackNum�E�E�E�R���{�̎�ނ𔻕ʂ���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::PlayComboAnimtion_Sword(int _attackNum)
{
	// �R���{�s�̎��X���[
	if (!CanCombo) { return; }

	// �W�����v���ł͂Ȃ��Ƃ�
	if (State != EPlayerState::Jump)
	{
		// �U���̃A�j���[�V�����Đ�
		PlayAnimation(PlayerAnimation.ComboAttack[_attackNum], ComboStartSectionNames[ComboIndex]);

	}
	// �W�����v�U��
	else
	{
		// �W�����v�U���̃A�j���[�V�����Đ�
		PlayAnimation(PlayerAnimation.AirAttack);

		// �W�����v�U���̎���2�i�ڂ̍U�����X�L�b�v
		++ComboIndex;
	}

	// ���X�g�A�^�b�N�܂ŃR���{�p��
	if (ComboStartSectionNames[ComboIndex] != ComboStartSectionNames.Last())
	{
		++ComboIndex;
	}

	// �X�e�[�g���U������
	State = EPlayerState::Attack;
}


/*
 * �֐����@�@�@�@�FPlayComboAnimtion_Lance()
 * �������e�@�@�@�F���̃R���{
 * �����P�@�@�@�@�Fint _attackNum�E�E�E�R���{�̎�ނ𔻕ʂ���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::PlayComboAnimtion_Lance(int _attackNum)
{
	// ���̎��Ɠ����A�j���[�V�������s��
	PlayComboAnimtion_Sword(_attackNum);
}


/*
 * �֐����@�@�@�@�FPlayComboAnimtion_Gun()
 * �������e�@�@�@�F�e�̍U���A�j���[�V����
 * �����P�@�@�@�@�Fint _attackNum�E�E�E�R���{�̎�ނ𔻕ʂ���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::PlayComboAnimtion_Gun(int _attackNum)
{
	// �R���{�s�̎��X���[
	if (!CanCombo) { return; }

	// �R��U���t���O�I��
	if (_attackNum == 0)
	{
		IsKicking = true;
	}

	// �Ή�����A�j���[�V�����Đ�
	PlayAnimation(PlayerAnimation.GunAttack[_attackNum]);

	// �X�e�[�g���U������
	State = EPlayerState::Attack;
}


/*
 * �֐����@�@�@�@�FRotateCharacter()
 * �������e�@�@�@�F�v���C���[�̃X�e�[�^�X������
 * �����P�@�@�@�@�Ffloat _nowInput_X�E�E�E���݂̈ړ����͒l
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::RotateCharacter(float _nowInput_X)
{
	// ���͂��Ȃ��ꍇ�͉������Ȃ�
	if (_nowInput_X == 0) { return; }

	// �E�������Ă��邩�m�F
	bool bLookRight = (_nowInput_X != 1.f) ? (true) : (false);

	// ��]
	ActionAssistComp->OwnerParallelToCamera(bLookRight);
}


/*
 * �֐����@�@�@�@�FSlowDownDeathAnimationRate()
 * �������e�@�@�@�F���S���A�j���[�V���������L�΂�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SlowDownDeathAnimationRate()
{
	// ���b�V���̃A�j���[�V�������x��x������
	GetMesh()->GlobalAnimRateScale = DeadAnimRate;


	// �v���C���[���폜����܂ł̃^�C�}�[���Z�b�g
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &APlayerBase::CallControllerFunc_DestroyPlayer, DeadToGameOverTime, false);
}



/*
 * �֐����@�@�@�@�FCallControllerFunc_DestroyPlayer()
 * �������e�@�@�@�F���S���̃Q�[�����[�h���̊֐��Ăяo��
 * �@�@�@�@�@�@�@�@�c�@���c���Ă��烊�X�|�[��,�Ȃ��Ȃ�����v���C���[�폜
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::CallControllerFunc_DestroyPlayer()
{
	// �R���g���[���[����v���C���[���폜
	if (!PlayerController) { return; }

	// �c�@�����邤���̓��X�|�[������ȊO�̓Q�[���I�[�o�[
	(PlayerController->GetRemainingLives() > 0) ? (PlayerController->RespawnPlayer()) : (PlayerController->DestroyPlayer());
}


/*
 * �֐����@�@�@�@�FResetAllAttackFlags()
 * �������e�@�@�@�F�U���Ɋւ���t���O�����ׂă��Z�b�g
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::ResetAllAttackFlags()
 {
	 // �U�����̃t���O���Z�b�g
	 CanCombo = true;
	 IsKicking = false;
	 ComboIndex = 0;
	 State = EPlayerState::Idle;
 }


/*
 * �֐����@�@�@�@�FOnDeath()
 * �������e�@�@�@�F�v���C���[�̔�_���[�W���̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::OnDamage(bool _isLastAttack)
{
	// ��_���[�W��Ԃ�
	State = EPlayerState::Damage;

	// ���G�ɂ���
	SetInvincibilityOn();

	// �G�̃R���{���ŏI�i���������K������𗎂Ƃ�
	if (_isLastAttack)
	{
		DefaultDamageLimit = 0;
	}

	// ���G�������Ԋi�[�p
	float InvincibilityReleaseTime;

	// ��_���[�W�A�j���[�V����
	if (DefaultDamageLimit <= 0)
	{
		// �������G���Ԃ�K�p
		InvincibilityReleaseTime = InvincibilityTime_Long;

		// ����𗎂Ƃ�
		DetachWeapon(false);

		// �m�b�N�o�b�N�A�j���[�V�����Đ�
		PlayAnimation(PlayerAnimation.KnockBack);
	}
	else
	{
		// �Z�����G���Ԃ�K�p
		InvincibilityReleaseTime = InvincibilityTime_Long;

		// �U�����󂯂�
		--DefaultDamageLimit;

		// �̂�����A�j���[�V�����Đ�
		PlayAnimation(PlayerAnimation.TakeDamage);
	}

	// ��莞�Ԍ�ɖ��G����
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &APlayerBase::SetInvincibilityOff, InvincibilityReleaseTime, false);
}

/*
 * �֐����@�@�@�@�FOnDeath()
 * �������e�@�@�@�F�v���C���[�̎��S���̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
 void APlayerBase::OnDeath()
 {
	 // ���S��Ԃ�
	 State = EPlayerState::Death;

	 // �R���W�������I�t��
	 SetActorEnableCollision(true);

	 // ���S�A�j���[�V�����Đ�
	 PlayAnimation(PlayerAnimation.Death);
 }


 /*
  * �֐����@�@�@�@�FContinuationCombo()
  * �������e�@�@�@�F�R���{�̌p��
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::ContinuationCombo()
 {
	 CanCombo = true;
 }


 /*
  * �֐����@�@�@�@�FResetCombo()
  * �������e�@�@�@�F�R���{���Z�b�g
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::ResetCombo()
 {
	 // �U�����̃t���O�����ׂă��Z�b�g
	 ResetAllAttackFlags();
 }


 /*
  * �֐����@�@�@�@�FStopMontage()
  * �������e�@�@�@�F�A�j���[�V�������~�߂�
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::StopMontage()
 {
	 //�A�j���[�V�������~�߂�
	 GetMesh()->GetAnimInstance()->StopSlotAnimation();
 }


 /*
* �֐����@�@�@�@�FSetInvincibilityOn()
* �������e�@�@�@�F��_���[�W���ɖ��G��
* �߂�l�@�@�@�@�F�Ȃ�
*/
 void APlayerBase::SetInvincibilityOn()
 {
	 // ���G���������ăX�e�[�g���_���[�W��Ԃ�
	 IsInvincibility = true;
 }


 /*
 * �֐����@�@�@�@�FSetInvincibilityOff()
 * �������e�@�@�@�F���G���Ԍ�ɖ��G����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
 void APlayerBase::SetInvincibilityOff()
 {
	 // ���G���������ăX�e�[�g��ʏ��Ԃ�
	 IsInvincibility = false;
 }

 /*
  * �֐����@�@�@�@�FTakedDamage()
  * �������e�@�@�@�F�v���C���[�̔�_���[�W����
  * �����P�@�@�@�@�Ffloat _damage�E�E�E��_���[�W��
  * �����Q�@�@�@�@�Fbool _isLastAttack�E�E�E�R���{�̍ŏI���ǂ���
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::TakedDamage(float _damage, bool _isLastAttack /*= false*/)
 {
	 if (State == EPlayerState::Death || IsInvincibility || AbsolutelyInvincible) { return; }

	 // ����������Ă��Ȃ��Ƃ��ɍU�����󂯂��玀�S
	 if (!GetWeapon() && State != EPlayerState::Death)
	 {
		 OnDeath();
	 }
	 // ����������Ă���Ƃ�
	 else
	 {
		 // �U�����̃t���O�����ׂă��Z�b�g
		 ResetAllAttackFlags();

		 // �_���[�W����
		 OnDamage(_isLastAttack);
	 }

	 // �q�b�g�G�t�F�N�g����
	 ActionAssistComp->SpawnEffect(HitEffect, GetActorLocation());
 }

 //-----------------------�f�o�b�O�p�֐�-----------------------------------------------------------
 /*
  * �֐����@�@�@�@�FQuitGame()
  * �������e�@�@�@�FESC:�Q�[���I��
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::QuitGame()
 {
	 // �Q�[���I��
	 UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
 }


 /*
  * �֐����@�@�@�@�FQuitGame()
  * �������e�@�@�@�FF1:�Q�[�����X�^�[�g
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::RestartGame()
 {
	 if (!pGameMode) { return; }

	 // �Q�[�����[�h���烊�X�^�[�g�������Ă�
	 pGameMode->RestartGame();
 }


 /*
 * �֐����@�@�@�@�FStartInGame()
 * �������e�@�@�@�FF2:�C���Q�[������X�^�[�g
 * �߂�l�@�@�@�@�F�Ȃ�
 */
 void APlayerBase::StartInGame()
 {
	 if (!pGameMode) { return; }

	 // �Q�[�����[�h����C���Q�[���J�n�������Ă�
	 pGameMode->InGameStart();
 }


 /*
  * �֐����@�@�@�@�FSetGamePause()
  * �������e�@�@�@�FF3:�Q�[���|�[�Y
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::SetGamePause()
 {
	 if (!pGameMode) { return; }

	 // �|�[�Y�̐؂�ւ�
	 bool IsPaused = !(pGameMode->IsPaused());

	 // �|�[�Y�ݒ�
	 pGameMode->SetGamePause(IsPaused);
 }


 /*
 * �֐����@�@�@�@�FExitBattleArea()
 * �������e�@�@�@�FF4:�o�g���G���A����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
 void APlayerBase::ExitBattleArea()
 {
	 if (!pGameMode) { return; }

	 pGameMode->DestroyBattleAreaEnemy_Debug();
 }


 /*
  * �֐����@�@�@�@�FSetAbsolutelyInvincible()
  * �������e�@�@�@�FF6:���G
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::SetAbsolutelyInvincible()
 {
	 // ���G�̃I���I�t
	 AbsolutelyInvincible = !AbsolutelyInvincible;
 }


 /*
  * �֐����@�@�@�@�FSetDeath()
  * �������e�@�@�@�FF7:�f�o�b�O�p���S
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::SetDeath()
 {
	 // ����𗎂Ƃ�
	 DetachWeapon(false);

	 TakedDamage(0);
 }

 //------------------------------------------------------------------------------------------------

 /*
  * �֐����@�@�@�@�FPlayAnimation()
  * �������e�@�@�@�F�v���C���[�̃A�j���[�V�����Đ�(�Đ����͑���s��)
  * �߂�l�@�@�@�@�F�Ȃ�
  */
 void APlayerBase::PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName /*= "None"*/, float _playRate /*= 1.f*/)
 {
	 // �U���s��
	 CanCombo = false;

	 // �Đ�����A�j���[�V�������i�[
	 UAnimMontage* toPlayAnimMontage = _toPlayAnimMontage;

	 // �A�j���[�V�����Đ�
	 if (toPlayAnimMontage != nullptr)
	 {
		 PlayAnimMontage(_toPlayAnimMontage, _playRate, _startSectionName);
	 }
 }


