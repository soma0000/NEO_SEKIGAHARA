// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"
#include "NEO/GameSystem/NEOGameMode.h"
#include "NEO/AnimationSystem/CharacterAnimInstance.h"



// Sets default values
//�R���X�g���N�^+�ϐ��̏�����
AOdaBase::AOdaBase() :
	SpawnTimer(0),						//---------------------------
	FlameCounter(0),					//�^�C�}�[�n
	TimeCounter(0),						//---------------------------
	isMove(true),
	OdaMoveEnum(ECPPOdaEnum::Movie),	//�񋓌^
	HoldOdaMoveEnum(ECPPOdaEnum::Stay1),
	Attack1Delay(0),					//�ߐڍU���̑ҋ@����
	RandomNum(-1),
	BladeDamage(0.f),
	ShockWaveDamage(10.f),
	UltShockWaveDamage(5.f),
	UltDamage(30.f),
	OdaSpeed{1.5f,2.f,.5f},
	OneMoreShockWave(false),
	ChangeFlontTimer(200),
	isShockWaveSpawnTiming(false),
	isUltShotTiming(false),
	isUltShot(true),
	UltTimer(550),
	isNotAttackNow(false),
	NotAttackCount(0),
	Attack1WaitingTime(60),
	iAnimTime{ 0,0,0,0,0,0,0 },
	bStartMovieOnce(true),
	MovieMove(false),
	SwordFirstDamage(5),
	HoldDamageAdd(0),
	SwordAddDamage(5),
	EnemyOneTimeSpawn(0),
	bIsAttacked(false),
	Health(1000.f),
	MaxHealth(1000.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Assist Create
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("ActionAssist"));

}

// Called when the game starts or when spawned
void AOdaBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();

	//���[�V�����̒��������(*60�͕b����t���[���ɕϊ����邽��)
	iAnimTime.BossKnock = (int)AnimMontage_BossKnockMontage->GetPlayLength() * 60.f;
	iAnimTime.Attack1 = (int)AnimMontage_BossAttack1->GetPlayLength() * 60.f;
	iAnimTime.Attack2 = ((int)AnimMontage_BossAttack2->GetPlayLength() + 1) * 60.f;
	iAnimTime.Ultimate = ((int)AnimMontage_BossUltimate->GetPlayLength() + 1) * 60.f;
	iAnimTime.BlowAway = (int)AnimMontage_BossBlowAway->GetPlayLength() * 60.f;
	iAnimTime.Death = (int)AnimMontage_BossDeath->GetPlayLength() * 60.f;
	iAnimTime.StartMovie = (int)AnimMontage_BossStartMovie->GetPlayLength() * 60.f;


	//�ϐ��̏�����
	isMotionReset(true);

	NobunagaMovement = (GetCharacterMovement());
	PlayerChara = nullptr;

	//�f�B���C����莞�Ԃ��U�����󂯂��珈����؂�ւ���
	SpawnDelay = true;

	//�{�X��HP����C�ɍ��Ȃ��悤��
	isBossHPRock = true;

	
}


// Called to bind functionality to input
void AOdaBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called every frame
void AOdaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�v���C���[�����X�|�[���������ɒ��g���Ȃ��Ȃ��Ă��܂��̂ōX�V
	AActor* Player = GetPlayer();
	//�v���C���[���Ȃ������炱��ȍ~�̏������X���[����
	if (Player == nullptr)  return;

	//���ꂷ�����Ƃ��͓����Ȃ��悤��
	if (FVector::Dist(GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()) <= 4000.f)
	{
		//�t���[�����Ƃɉ��Z����
		FlameCounter++;

		//���[�r�[���Ȃ��΂�
		if (isMove == false)
		{
			return;
		}
		//�������v���C���[�̕��Ɍ�����(30�t���[�����ɍX�V)
		if (FlameCounter % 30 == 0 )
		{
			//�v���C���[�̕�������
			ToPlayerRotate();
		}

		if (isDamageHit == true)
		{
			//�v���C���[�̕�������
			ToPlayerRotate();
		}
		//���������
		//X��
		BossPosX = FVector(GetActorLocation().X, 0.f, 0.f);
		PlayerPosX = FVector(Player->GetActorLocation().X, 0.f, 0.f);

		//Y��
		BossPosY = FVector(0.f, GetActorLocation().Y, 0.f);
		PlayerPosY = FVector(0.f, Player->GetActorLocation().Y, 0.f);

		//�J�E���^�[�N��
		TimeCounter++;

		//�̗͂������ȉ��ɂȂ�����
		if (Health < MaxHealth / 2.f)
		{
			//���ڂ������߂̃^�C�}�[���N��
			UltTimer++;
			if (UltTimer % 600 == 0)//600�t���[����ɕK�E�����Ă�悤�ɂ���
			{
				//�K�E�Z�����Ă�悤�ɂ���
				isUltShot = false;
			}
		}


		//��Ԃ��Ƃɓ�����؂�ւ���
		switch (OdaMoveEnum)
		{
			//�ҋ@,����
		case ECPPOdaEnum::Stay1:
			OdaStay1(TimeCounter);
			break;

			//�ҋ@,����
		case ECPPOdaEnum::Stop:

			if (FlameCounter > 30)
			{
				if (HoldOdaMoveEnum == ECPPOdaEnum::Attack1)
				{
					OdaMoveEnum = ECPPOdaEnum::Attack1;
				}
				else
				{
					OdaMoveEnum = ECPPOdaEnum::Stay1;
				}
			}
			break;

		case ECPPOdaEnum::Movie:
			OdaMovie(TimeCounter);
			break;

			//���ʃ_�b�V��
		case ECPPOdaEnum::Moveflont:
			//�E���������������Ő�����ς���
			if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
			{
				BossMoveY(OdaSpeed.Dash);
			}
			else
			{
				BossMoveY(-OdaSpeed.Dash);
			}
			//��莞�Ԃ�������
			if (TimeCounter % 15 == 0)
			{
				//�ҋ@�ɖ߂�
				BacktoStayCase();
			}
			break;

			//�w�ʃ_�b�V��
		case ECPPOdaEnum::MoveBack:
			//�E���������������Ő�����ς���
			if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
			{
				BossMoveY(OdaSpeed.Back);
			}
			else
			{
				BossMoveY(-OdaSpeed.Back);
			}
			//��莞�Ԃ�������
			if (TimeCounter % 30 == 0)
			{
				//�ҋ@�ɖ߂�
				BacktoStayCase();
			}
			//���C���΂��Č������̕ǂɓ���������
			else if (WallCheck(-100.f))
			{
				//������ς���
				IsWallCheck(ECPPOdaEnum::MoveBack);
			}
			break;

			//�U���P
		case ECPPOdaEnum::Attack1:
			if (TimeCounter % Attack1WaitingTime == 0)
			{
				OdaAttack1(TimeCounter);
			}
			break;

			//�U���Q
		case ECPPOdaEnum::Attack2:
			OdaAttack2(TimeCounter);

			break;

			//�K�E�Z
		case ECPPOdaEnum::Ultimate:
			OdaUlt(TimeCounter);
			break;

		default:
			break;
		}
	}
}
///*
// * �֐����@�@�@�@�FNomalMode()
// * �������e�@�@�@�F�ʏ�U���p�֐�
// * �߂�l�@�@�@�@�F�Ȃ�
// */
//void AOdaBase::NomalMode()
//{
//	//��Ԃ��Ƃɓ�����؂�ւ���
//	switch (OdaMoveEnum)
//	{
//		//�ҋ@,����
//	case ECPPOdaEnum::Stay1:
//		OdaStay1(TimeCounter);
//		break;
//
//		//�ҋ@,����
//	case ECPPOdaEnum::Stop:
//
//		if (FlameCounter > 30)
//		{
//			if (HoldOdaMoveEnum == ECPPOdaEnum::Attack1)
//			{
//				OdaMoveEnum = ECPPOdaEnum::Attack1;
//			}
//			else
//			{
//				OdaMoveEnum = ECPPOdaEnum::Stay1;
//			}
//		}
//		break;
//
//		//�U���P
//	case ECPPOdaEnum::Attack1:
//		if (TimeCounter % Attack1WaitingTime == 0)
//		{
//			OdaAttack1(TimeCounter);
//		}
//		break;
//
//		//�U���Q
//	case ECPPOdaEnum::Attack2:
//		OdaAttack2(TimeCounter);
//
//		break;
//
//	}
//}
//
///*
// * �֐����@�@�@�@�FUltMode()
// * �������e�@�@�@�F�K�E�Z�p�֐�
// * �߂�l�@�@�@�@�F�Ȃ�
// */
//void AOdaBase::UltMode()
//{
//	//�̗͂������ȉ��ɂȂ�����
//	if (Health < MaxHealth / 2.f)
//	{
//		//���ڂ������߂̃^�C�}�[���N��
//		UltTimer++;
//		if (UltTimer % 600 == 0)//600�t���[����ɕK�E�����Ă�悤�ɂ���
//		{
//			//�K�E�Z�����Ă�悤�ɂ���
//			isUltShot = false;
//		}
//	}
//}


/*
 * �֐����@�@�@�@�FToPlayerRotate()
 * �������e�@�@�@�FY�����������Ăǂ������ɂ��邩�𔻒肷��֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::ToPlayerRotate()
{
	bool LookRight;

	if (isNowAttacking != true)
	{
		//�{�X���v���C���[�̉E�ɂ���Ƃ�
		if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
		{
			LookRight = true;
		}
		else
		{
			LookRight = false;
		}

		ActionAssistComp->OwnerParallelToCamera(LookRight);
	}
	if (isDamageHit == true)
	{
		//�{�X���v���C���[�̉E�ɂ���Ƃ�
		if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
		{
			LookRight = true;
		}
		else
		{
			LookRight = false;
		}
		//�U��������������p�̕ϐ������ɖ߂�
		isDamageHit = false;
		ActionAssistComp->OwnerParallelToCamera(LookRight);
	}

}


/*
 * �֐����@�@�@�@�FOdaStay1()
 * �������e�@�@�@�F�U�������ҋ@���Ă���Ƃ��̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaStay1(int Timer)
{
	//����Ă�����
	if (FVector::Dist(BossPosY, PlayerPosY) >= 700.f)
	{
		//�O�����ɓ�������
		if (RandomNum == 0)
		{
			BossMoveY(OdaSpeed.Flont);
		}
		else if (RandomNum == 1)
		{
			BossMoveY(-OdaSpeed.Flont);
		}

	}

	//���[�V�����𗬂���Ԃ��ǂ���(isMotionPlaying��true�ɂ���)
	isMotionReset(true);

	//�]��ɂ�����Ă�����O�ɑ���
	if (FVector::Dist(BossPosY, PlayerPosY) >= 700.f)
	{
		OdaMoveEnum = ECPPOdaEnum::Moveflont;
	}

	if (FVector::Dist(BossPosX, PlayerPosX) <= 50.f)
	{

		if (isUltShot == false)
		{
			//�K�E�Z
			OdaMoveEnum = ECPPOdaEnum::Ultimate;
			isUltShot = true;
			UltTimer = 0;

			//random�Œ�̃��Z�b�g
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		//�v���C���[�Ƃ̋����̔���
		else if (FVector::Dist(BossPosY, PlayerPosY) <= 200.f)
		{
			//�ߐڍU��
			OdaMoveEnum = ECPPOdaEnum::Attack1;
			TimeCounter = 0;
			//�����U���̃f�B���C�̒l�������Ă��Ȃ�������
			if (Attack1Delay != 0)
			{
				Attack1Delay = 10;
			}

			//random�Œ�̃��Z�b�g
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		//����Ă�����
		else if (FVector::Dist(BossPosY, PlayerPosY) >= 300.f)
		{
			//������
			OdaMoveEnum = ECPPOdaEnum::Attack2;
			TimeCounter = 0;
			//random�Œ�̃��Z�b�g
			if (RandomNum != -1)
			{
				RandomNum = -1;
			}
		}
		else
		{
			//�������킹�ɍs��
			OdaMove1(Timer, 60);
			Attack1Delay = 15;
		}
		//-----------------------------------------------------------------------------------------------------------------------------------------------

	}
	else
	{
		//�������킹�ɍs��
		if (GetActorLocation().X < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().X)
		{
			BossMoveX(OdaSpeed.Flont);
		}
		else
		{
			BossMoveX(-OdaSpeed.Flont);
		}

	}
}

void AOdaBase::InMovie()
{
	OdaMoveEnum = ECPPOdaEnum::Movie;
	OdaMovie(1);
}

void AOdaBase::OdaMovie(int Timer)
{
	if (bStartMovieOnce)
	{
		//�ŏ��̌��������ʂ������悤�ɏC��
		ToPlayerRotate();
		//���[�r�[�p�A�j���[�V�����𗬂�
		PlayAnimMontage(AnimMontage_BossStartMovie);
		//��x�����Ȃ���
		bStartMovieOnce = false;
	}
	//���[�r�[�I����ҋ@�ɖ߂�
	if (Timer > iAnimTime.StartMovie)
	{
		OdaMoveEnum = ECPPOdaEnum::Stay1;
	}
	if (Timer % 260 == 0)
	{
		MovieMove = true;
	}
	if (MovieMove)
	{
		//������
		if (GetActorLocation().Y < UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y + 3.5f, GetActorLocation().Z), true);
		}
	}
}

/*
 * �֐����@�@�@�@�FOdaMove1()
 * �������e�@�@�@�F�������킹��ȂǓ����ɂ��Ă̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaMove1(int DeltaTime, int StopTimer)
{
	if (isAttack1Waiting)
	{
		isAttack1Waiting = false;
	}

	//�������킹�ɍs��
	//random�̒��ɒl�������Ă��邩(-1�������Ă��Ȃ����)
	if (RandomNum == -1)
	{
		//random��0��1������
		RandomNum = FMath::RandRange(0, 1);
	}

	//�O�����ɓ�������
	if (RandomNum == 0)
	{
		BossMoveY(OdaSpeed.Flont);
	}
	else if (RandomNum == 1)
	{
		BossMoveY(-OdaSpeed.Flont);
	}

}
/*
 * �֐����@�@�@�@�FIsWallCheck()
 * �������e�@�@�@�F�����点�����Ȃ��悤�ɂ���֐�
 * ����  �@�@�@�@�F���݂̏��(�U����)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::IsWallCheck(ECPPOdaEnum BossState) {

	//�������U����Ȃ�
	if (BossState == ECPPOdaEnum::Moveflont)
	{
		//�����ҋ@�Ɉڂ�
		BacktoStayCase();
	}
	//�K�E�Z�Ȃ�
	if (BossState == ECPPOdaEnum::Ultimate) {
		isUltShotTiming = true;
	}
	//�m�b�N�o�b�N���Ȃ�
	if (BossState == ECPPOdaEnum::MoveBack)
	{
		//�����ҋ@�Ɉڂ�
		BacktoStayCase();
	}
}

/*
 * �֐����@�@�@�@�FOdaAttack1()
 * �������e�@�@�@�F�ߋ����U�������邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaAttack1(int Timer) {
	//UKismetSystemLibrary::PrintString(this, "Attack1", true, true, FColor::Cyan, 2.f, TEXT("None"));

	if (Attack1Delay == 0)
	{
		//0���Ɗ���؂�Ȃ��̂�1�������Ă���
		Attack1Delay = 1;
	}

	//�ݒ肵�����Ԃ𒴂�����
	if (Timer % Attack1Delay == 0)
	{
		//���[�V����(�A�j���[�V����)���N��������
		if (isMotionPlaying.isAttack1Play == true)
		{
			//�A�j���[�V�����𗬂�
			AnimationPlay(AnimMontage_BossAttack1,&isMotionPlaying.isAttack1Play);
			//�{�X���m�b�N�o�b�N���Ȃ��悤�ɂ���
			isBossHPRock = false;
			//�f�B���C�����Z�b�g����
			Attack1Delay = 0;
			//�U������Ȃ��悤�ɂ���
			isNowAttacking = true;
		}
	}

	//�A�j���[�V�������I��������
	if (Timer > iAnimTime.Attack1)
	{
		//�U�������悤�ɂ���
		isNowAttacking = false;
		isDamageHit = false;
		//�X�e�[�g��؂�ւ���
		BacktoStayCase();
		//���Z�b�g
		NotAttackCount = 0;
		//�m�b�N�o�b�N���ɍU�����[�V�����ɓ����HP���b�N���쓮�������Ă��܂��̂ł����Ő؂�ւ���
		if (isHPLock == true)
		{
			HPLock();
		}
	}
}


/*
 * �֐����@�@�@�@�FOdaAttack2()
 * �������e�@�@�@�F�������U�������邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaAttack2(int Timer) {
	//UKismetSystemLibrary::PrintString(this, "Attack2", true, true, FColor::Cyan, 2.f, TEXT("None"));

	//���[�V����(�A�j���[�V����)���N��������
	if (isMotionPlaying.isAttack2Play == true)
	{
		//�_���[�W�l����
		BladeDamage = ShockWaveDamage;
		//�A�j���[�V�����𗬂�
		AnimationPlay(AnimMontage_BossAttack2,&isMotionPlaying.isAttack2Play);
		//�U������Ȃ��悤�ɂ���
		isNowAttacking = true;
	}


	//�A�j���[�V�����̍Ō�ɏՌ��g���o�������̂�Notify���g���Đ��䂷��(ShockWaveSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isShockWaveSpawnTiming == true)
	{
		//�A�N�^�[�̃X�|�[������(ShockWaveSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isShockWaveSpawnTiming = false;
	}

	//�A�j���[�V�������I��������
	if (Timer > iAnimTime.Attack2)
	{
		//�U�������悤�ɂ���
		isNowAttacking = false;

		//�O�_�b�V�����Ă݂�
		OdaMoveEnum = ECPPOdaEnum::Moveflont;

		//�؂�ւ���ɂ������ĕϐ�������������
		TimeCounter = 0;
		//���Z�b�g
		NotAttackCount = 0;
		//�m�b�N�o�b�N���ɍU�����[�V�����ɓ����HP���b�N���쓮�������Ă��܂��̂ł����Ő؂�ւ���
		if (isHPLock == true)
		{
			HPLock();
		}
	}
}
/*
 * �֐����@�@�@�@�FOdaUlt()
 * �������e�@�@�@�F�K�E�Z�U�������邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::OdaUlt(int Timer)
{

	if (Timer % 30 == 0 && isUltShotTiming != true)
	{
		isUltShotTiming = true;
	}
	//�������킹�ɍs��
	if (isUltShotTiming != true)
	{
		//�U������Ȃ��悤�ɂ���
		isNowAttacking = true;
		//���C���΂��Č������̕ǂɓ���������
		if (WallCheck(-100.f))
		{
			//������ς���
			IsWallCheck(ECPPOdaEnum::Ultimate);
		}
		//���
		if (GetActorLocation().Y > UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Y)
		{
			BossMoveY(OdaSpeed.Flont);
		}
		else
		{
			BossMoveY(-OdaSpeed.Flont);
		}
	}

	//���[�V����(�A�j���[�V����)���N��������
	else if (isMotionPlaying.isUltPlay == true)
	{
		//�A�j���[�V�����𗬂�
		AnimationPlay(AnimMontage_BossUltimate,&isMotionPlaying.isUltPlay);
		//�{�X���m�b�N�o�b�N���Ȃ��悤�ɂ���
		BossHPRock();
	}

	//�Ռ��g���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isShockWaveSpawnTiming == true)
	{
		//�_���[�W�l����
		BladeDamage = UltShockWaveDamage;
		//�A�N�^�[�̃X�|�[������(ShockWaveSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(ShockWaveSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isShockWaveSpawnTiming = false;
	}

	//�A�j���[�V�����̍Ō�ɕK�E�Z���o�������̂�Notify���g���Đ��䂷��(UltSpawnFlagChange�ɂĕϐ��̒��g��ύX)
	if (isUltSpawnTiming == true)
	{
		//�A�N�^�[�̃X�|�[������(UltSpawn�̓u���[�v�����g��Őݒ�)
		GetWorld()->SpawnActor<AActor>(UltSpawn, GetActorTransform());
		//��x�����X�|�[�����������̂Ő؂�ւ��Ă���
		isUltSpawnTiming = false;
	}



	//�A�j���[�V�������I��������
	if (Timer > iAnimTime.Ultimate)
	{
		//�U�������悤�ɂ���
		isNowAttacking = false;

		//�X�e�[�g��؂�ւ���
		OdaMoveEnum = ECPPOdaEnum::Moveflont;
		//�ҋ@���[�V�����ɐ؂�ւ���
		Cast < UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetCharacterState(ECharacterState::State_Idle);


		//�؂�ւ���ɂ������ĕϐ�������������
		TimeCounter = 0;
		isUltShotTiming = false;
	}
}
/*
 * �֐����@�@�@�@�FAttackFlagChange()
 * �������e�@�@�@�FAnimNotify�ɂĕύX�A�U���̃t���O��on,off�ɂ��Ă̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::AttackFlagChange()
{
	//���]������
	IsAttackNow = !IsAttackNow;
}
/*
* �֐����@�@�@�@�FisMotionReset()
* �������e�@�@�@�F���[�V�������N�������ǂ����̕ϐ������Z�b�g����ׂ̊֐�
* �߂�l�@�@�@�@�F�Ȃ�
*/
void AOdaBase::isMotionReset(bool _isPlaying)
{
	isMotionPlaying.isAttack1Play = _isPlaying;
	isMotionPlaying.isAttack1Play = _isPlaying;
	isMotionPlaying.isAttack2Play = _isPlaying;
	isMotionPlaying.isUltPlay = _isPlaying;
	isMotionPlaying.isDeathPlay = _isPlaying;
	isMotionPlaying.isMoviePlay = _isPlaying;
}


/*
* �֐����@�@�@�@�FsetRootMotion()
* �������e�@�@�@�F���[�g���[�V����(�^��)�����邽�߂̊֐�
* �߂�l�@�@�@�@�F�Ȃ�
*/
void AOdaBase::setRootMotionMove(float speed)
{
	if (GetActorRotation() == FRotator(0.f,90.f,0.f))
	{
		BossMoveY(speed);
	}
	else
	{
		BossMoveY(-speed);
	}
}

/*
 * �֐����@�@�@�@�FShockWaveSpawnFlagChange()
 * �������e�@�@�@�F�Ռ��g���X�|�[�������邽�߂̕ϐ���؂�ւ���ׂ̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::ShockWaveSpawnFlagChange()
{
	//�Ռ��g���o��������ׂ̕ϐ���true�ɕς���
	isShockWaveSpawnTiming = true;
	//����IsAttackNow��true�ɂȂ��Ă�����
	if (IsAttackNow == true)
	{
		//false�ɂ���
		IsAttackNow = false;
	}
}

/*
 * �֐����@�@�@�@�FUltSpawnFlagChange()
 * �������e�@�@�@�F�K�E�Z���X�|�[�������邽�߂̕ϐ���؂�ւ���ׂ̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::UltSpawnFlagChange(TSubclassOf<AActor>_UltSpawn)
{
	//�����̒��ɒl���͂����Ă�����
	if(UltSpawn != NULL)
	{
		UltSpawn = _UltSpawn;
	}
	//�K�E�Z���o��������ׂ̕ϐ���true�ɕς���
	isUltSpawnTiming = true;
	//����IsAttackNow��true�ɂȂ��Ă�����
	if (IsAttackNow == true)
	{
		//false�ɂ��Ă݂�
		IsAttackNow = false;
	}
}

/*
 * �֐����@�@�@�@�FApplyDamage()
 * �������e�@�@�@�F�_���[�W���󂯂����̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::TakedDamage(float Damage , bool _bLastAttacked)
{
	if (FlameCounter >= 100)
	{
		FlameCounter = 0;
	}
	//���݂̏�Ԃ�ۑ�
	HoldOdaMoveEnum = OdaMoveEnum;
	//�������~�߂�
	OdaMoveEnum = ECPPOdaEnum::Stop;

	if (isHPLock != true)
	{
		//�{�X�̃X�|�[�����ҋ@���Ԓ��ɍU�����󂯂���
		if (SpawnDelay == true)
		{
			//�t���O��؂�ւ���
			SpawnDelay = false;
			//�U���̃f�B���C���Z�b�g
			Attack1Delay = 10;
		}

		if (isMove)
		{
			Health -= Damage;
			if (isBossHPRock)
			{
				if (OdaMoveEnum == ECPPOdaEnum::Stay1)
				{
					if (FMath::RandRange(0, 100) <= 25)
					{


						//�m�b�N�o�b�N�A�j���[�V�����𗬂�
						PlayAnimMontage(AnimMontage_BossBlowAway);

						//�w�ʃ_�b�V���ɐ؂�ւ��Ďd�؂蒼��
						OdaMoveEnum = ECPPOdaEnum::MoveBack;

						//�A�j���[�V�����I����HP���b�N����������Stay�ɖ߂�
						FTimerManager& TimerManager = GetWorld()->GetTimerManager();
						TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BossHPRock, iAnimTime.BlowAway - 1, false);
						TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BacktoStayCase, iAnimTime.BlowAway, false);
					}


					else
					{
						//�m�b�N�o�b�N�̃A�j���[�V�����𗬂�
						PlayAnimMontage(AnimMontage_BossKnockMontage);
						// �A�j���[�V�����I����Stay�ɖ߂�
						FTimerManager& TimerManager = GetWorld()->GetTimerManager();
						TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BossHPRock, iAnimTime.BlowAway - 1, false);
						TimerManager.SetTimer(TimerHandle_KnockbackAnimationFinish, this, &AOdaBase::BacktoStayCase, iAnimTime.BlowAway, false);
					}
				}
			}
		}

		//HP��0�ɂȂ�����
		if (Health <= 0.f)
		{
			//�����邩�ǂ����̕ϐ���false�ɂ��Ă���
			if (isMove == true)
			{
				//�t���O���g���ă_���[�W���󂯂�A�j���[�V�����𓮂����Ȃ��悤�ɂ���
				isMove = false;
				//�ꉞ�A�j���[�V�������~�߂Ă���
				this->StopAnimMontage();

				//�Q�[���S�̂̑��x��x������
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .2f);

				//�A�j���[�V�����𗬂�
				AnimationPlay(AnimMontage_BossDeath, &isMotionPlaying.isDeathPlay);

				//�A�j���[�V�����̒����𔻒肵�Ă��̎��Ԃ��߂����玀�S�����̊֐��ɔ�΂��鏈��
				FTimerManager& TimerManager2 = GetWorld()->GetTimerManager();
				TimerManager2.SetTimer(TimerHandle_DeathToGameOver, this, &AOdaBase::Death, iAnimTime.Death, false);
			}
		}
		for (int i = 0; i < Enemy.Num(); i++)
		{
			if (Health <= Enemy[i].BossHPTiming && Enemy[i].isSpawn)
			{
				CallEnemy(i);
			}
		}
	}
}

/*
 * �֐����@�@�@�@�FCallEnemy()
 * �������e�@�@�@�F�G���Ăԏ���
 * �����P	�@�@ �F�v�f�ԍ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::CallEnemy(int _element)
{
	//TSubclassOf<class AActor> Enemy = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous(); // ��L�Őݒ肵���p�X�ɊY������N���X���擾
	EnemyActor.Add(GetWorld()->SpawnActor<AActor>(Enemy[_element].Enemy)); // �X�|�[������
	EnemyActor.Last()->SetActorLocation(Enemy[_element].SpawnPoint); // �m�F���₷���悤�ɍ��W��ݒ�
	Enemy[_element].isSpawn = false;									//������x��������Ȃ��悤�ɐ؂�ւ���
}

/*
 * �֐����@�@�@�@�FWorldTimeReturn()
 * �������e�@�@�@�F�����ƒx���܂܂��ƃX�g���X�����܂�̂Ō��̑��x�ɖ߂�(�A�j���[�V�����ʒm�ŊǗ�)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::WorldTimeReturn()
{
	//���łɓG������
	for (int i = 0; i < EnemyActor.Num(); i++)
	{
		if (EnemyActor[i] != NULL)
		{
			EnemyActor[i]->Destroy();
			EnemyActor[i] = NULL;
		}
	}
	//���Ԃ�߂�
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

/*
 * �֐����@�@�@�@�FBossHPRock()
 * �������e�@�@�@�F�{�X���m�b�N�o�b�N�����Ȃ��悤�ɂ��鏈��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BossHPRock()
{
	isBossHPRock = !isBossHPRock;
}



/*
 * �֐����@�@�@�@�FBacktoStayCase()
 * �������e�@�@�@�F�{�X�̍s����ҋ@�ɖ߂�����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BacktoStayCase()
{
	//�ҋ@�ɖ߂�
	OdaMoveEnum = ECPPOdaEnum::Stay1;
	//�؂�ւ���ɂ������ĕϐ�������������
	TimeCounter = 0;
	//�v���C���[�̕�������
	ToPlayerRotate();
	//����HP���b�N���O��Ă�����
	if (isBossHPRock != true)
	{
		//�_���[�W���󂯂����m�b�N�o�b�N����悤��
		BossHPRock();
	}
	if (!bStartMovieOnce)
	{
		bStartMovieOnce = true;
	}

	//�ҋ@���[�V�����ɐ؂�ւ���
	Cast < UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetCharacterState(ECharacterState::State_Idle);
}
void AOdaBase::AnimationPlay(class UAnimMontage* AnimMontage_AnimPlay ,bool* _isMotionPlaying)
{
	//�A�j���[�V�����𗬂�(���͉�)
	PlayAnimMontage(AnimMontage_AnimPlay);
	//��x�������������̂Ńt���O��؂�ւ���
	if (_isMotionPlaying != nullptr)
	{
		*_isMotionPlaying = false;
	}
}


void AOdaBase::BossKnockback()
{
	//
	if (!isBossHPRock)
	{
		isDamageHit = true;
		OdaMoveEnum = ECPPOdaEnum::MoveBack;
		if (isMove)
		{
			PlayAnimMontage(AnimMontage_BossBlowAway);
		}
	}
}

/*
 * �֐����@�@�@�@�FHPLock()
 * �������e�@�@�@�FHP���b�N�p�̃X�C�b�`��؂�ւ��鏈��(�v���C���[��)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::HPLock()
{
	//HP���b�N�p�̃X�C�b�`��؂�ւ���
	isHPLock = !isHPLock;
}

//�����֘A

/*
 * �֐����@�@�@�@�FBossMoveX()
 * �������e�@�@�@�F���ʕ����ɕ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BossMoveX(float Speed)
{
	//�㉺�����ɕ�������
	SetActorLocation(FVector(GetActorLocation().X + Speed, GetActorLocation().Y, GetActorLocation().Z));
	Cast < UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetCharacterState(ECharacterState::State_Move);

}

/*
 * �֐����@�@�@�@�FBossMoveYY()
 * �������e�@�@�@�F���ʕ����ɕ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::BossMoveY(float Speed)
{
	//���ʕ����ɕ�������
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y + Speed, GetActorLocation().Z));

	Cast < UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetCharacterState(ECharacterState::State_Move);
}

/*
 * �֐����@�@�@�@�FGetPlayer()
 * �������e�@�@�@�F�v���C���[���擾���鏈��
 * �߂�l�@�@�@�@�FAActor(�v���C���[�̏��)
 */
AActor* AOdaBase::GetPlayer()
{
	PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

/*
 * �֐����@�@�@�@�FtoPlayerAttacked()
 * �������e�@�@�@�F�v���C���[�ɑ΂��Ă�HP���b�N���邽�߂̊֐�
 * �߂�l�@�@�@�@�FAActor(�v���C���[�̏��)
 */
void AOdaBase::toPlayerAttacked()
{
	if (bIsAttacked)
	{
		bIsAttacked = false;
	}
}

/*
 * �֐����@�@�@�@�FIs2Combo()
 * �������e�@�@�@�F�������U���̃R���{�����邽�߂̊֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::Is2Combo()
{
	if (Combo2Counter == 0)
	{
		//HP��2/3�ȉ�����Ȃ�������ȉ��̏���
		if (Health > MaxHealth * (2.f / 3.f))
		{
			//�A�j���[�V�������~�߂�
			GetMesh()->GetAnimInstance()->StopSlotAnimation();
			//�ϐ������Z�b�g
			Combo2Counter = 0;

			Cast < UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetCharacterState(ECharacterState::State_Idle);

		}
		else
		{
			Combo2Counter++;
		}
	}
	else if (Combo2Counter == 1)
	{
		//HP��1/3�ȉ�����Ȃ�������ȉ��̏���
		if (Health > MaxHealth * (1.f / 3.f))
		{
			//�A�j���[�V�������~�߂�
			GetMesh()->GetAnimInstance()->StopSlotAnimation();
			//�ϐ������Z�b�g
			Combo2Counter = 0;
		}
		else
		{
			Combo2Counter++;
		}
	}
	else if (Combo2Counter)
	{
		//�ϐ������Z�b�g
		Combo2Counter = 0;
	}
}
/*
 * �֐����@�@�@�@�FLastAttack()
 * �������e�@�@�@�F�v���C���[���m�b�N�o�b�N�����邽�߂ɍŌ�̍U�����ǂ������Ƃ�֐�
 * �߂�l�@�@�@�@�Fbool(�Ō�̍U���Ȃ�true��������)
 */
bool AOdaBase::LastAttack()
{
	if (isMove)
	{
		if (Combo1Counter >= 3)
		{
			return true;
		}

		else if (Combo2Counter == 2)
		{
			return true;
		}
	}
	return false;
}



/*
 * �֐����@�@�@�@�FDeath()
 * �������e�@�@�@�F�̗͂�0�ȉ��ɂȂ����Ƃ��ɓ����֐�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::Death()
{
	//�Q�[�����[�h�ɂĂ��̃A�N�^����������
	ANEOGameMode* GameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (this != NULL)
		{
			//���݃G���[�ŗ�����̂ŕ��ʂɏ���
			//�Q�[�����[�h��p���ď���
			GameMode->DestroyEnemy(this, GetIsAreaEnemy());
		}
	}
}

// �U���������������̏���
void AOdaBase::Attack_Sword(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation)
{
	//���������Ă���A�N�^�[������(AActor*)
	TArray<AActor*> DamageInfo = HitActorAndLocation.Get<0>();

	//���������ꏊ���Ƃ��
	TArray<FVector> EffectPoint = HitActorAndLocation.Get<1>();

	for (AActor* ActorArray : DamageInfo)
	{
		if (ActorArray != nullptr)
		{
			if (ActorArray->ActorHasTag("Player"))
			{
				//�v���C���[�Ƀ_���[�W��^����(�����P�F�_���[�W�l�A����2�F�Ō�̍U�����ǂ���(�m�b�N�o�b�N�����邽��))
				Cast<ACharacterBase>(ActorArray)->TakedDamage(10.f,false);
				break;
			}
		}
	}

}


/*
 * �֐����@�@�@�@�FCheckOverlap()
 * �������e�@�@�@�F�{�b�N�X�R���W�����ɓ������Ă��邩�`�F�b�N
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::CheckOverlap()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FVector Start = BoxComp->GetComponentLocation();
	FVector End = Start;// + GetActorForwardVector() * 100.0f;
	FQuat Rot = BoxComp->GetComponentQuat();			// 
	FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxComp->GetUnscaledBoxExtent());

	bool isHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionBox, CollisionParams);

	if (isHit != true) { return; }

	if (false) {
		DrawDebugCapsule(GetWorld(), (Start + End) / 2,
			CollisionBox.GetCapsuleHalfHeight(),
			CollisionBox.GetCapsuleRadius(), Rot, FColor::Red, true, -1.0f, 0, 1.0f);
	}
}

/*
 * �֐����@�@�@�@�FPlayerOnOverlap()
 * �������e�@�@�@�F�v���C���[������������̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AOdaBase::PlayerOnOverlap(/*FHitResult& _HitResult*/)
{
	//�v���C���[��HP�����b�N�����炱�̏�����ʂ�
	if (bIsAttacked) {
		return;
	}

	//Weapon->SetCollision();

	//���Z�b�g
	bIsAttacked = true;
}
