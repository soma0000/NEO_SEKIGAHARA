// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h" 
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/NEOGameMode.h"
#include "NEO/GameSystem/GameSystem_BattleArea.h"
#include "NEO/CharacterSystem/ActionAssistComponent.h"
#include "NEO/CharacterSystem/PlayerSystem/PlayerBase.h"


// Sets default values
AEnamyBase::AEnamyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//UI Create

	bIsDeath = false;

	Damage = 5.0f;

	bHasPattern1Tag = Tags.Contains("pattern1");
	bHasPattern2Tag = Tags.Contains("pattern2");
	bHasPattern3Tag = Tags.Contains("pattern3");
	bHasWeponTag = Tags.Contains("HasWepon");



	MoveSpline = CreateDefaultSubobject<USplineComponent>(TEXT("MoveSpline"));
	MoveSpline->SetupAttachment(RootComponent);
	/*MovementSpline = CreateDefaultSubobject<USplineComponent>(TEXT("MovementSpline1"));
	MovementSpline->SetupAttachment(RootComponent);*/
}
/*
 * �֐����@�@�@�@�FAEnemyBase::DestroyEnemy
 * �������e�@�@�@�F�G�L�����N�^�[��j�󂵁A�Q�[�����[�h��DestroyEnemy���\�b�h���Ăяo���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::DestoryEnemy()
{
	ANEOGameMode* GameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->DestroyEnemy(this, GetIsAreaEnemy());

	}
	
}

/*
 * �֐����@�@�@�@�FAEnemyBase::BeginPlay
 * �������e�@�@�@�F�G�L�����N�^�[�̏������ƃo�g���G���A�̎擾���s���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	TArray<AActor*> TempBattleAreas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameSystem_BattleArea::StaticClass(), TempBattleAreas);

	for (AActor* Actor : TempBattleAreas) {
		AGameSystem_BattleArea* BattleArea = Cast<AGameSystem_BattleArea>(Actor);
		if (BattleArea) {

		}
	}
	
	bGetAway = false;
}
/*
 * �֐����@�@�@�@�FAEnemyBase::GetPlayer
 * �������e�@�@�@�F�v���C���[�|�[�����擾���ĕԂ��܂��B
 * �߂�l�@�@�@�@�F�v���C���[�|�[���̃|�C���^�iAActor*�j
 */
AActor* AEnamyBase::GetPlayer()
{
	APawn* PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

/*
 * �֐����@�@�@�@�FAEnemyBase::Tick
 * �������e�@�@�@�F���t���[���Ăяo����A�L�����N�^�[�̏�Ԃ�ʒu���X�V���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsNowDamage)
	{
		return;
	}


	//�L�����N�^�[�̈ʒu���擾
	FVector CharacterLocation = GetActorLocation();

	// �����̍��W���擾
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			APawn* Pawn = PlayerController->GetPawn();
			if (Pawn)
			{
				FVector MyLocation = Pawn->GetActorLocation();
				// �L�����N�^�[�̈ʒu�Ǝ����̈ʒu���r����Y�����O�ɂ��邩�ǂ����𔻒�
				bIsRotation = CharacterLocation.Y > MyLocation.Y;
				bIsRotationTag2 = CharacterLocation.X > MyLocation.X;
				//bIsRotation��true�Ȃ�
				if (!IsAnimationPlaying())
				{

					bool LookRight = false;

					if (ActorHasTag("pattern1"))
					{
						if (bIsRotation)
						{
							
							LookRight = true;
						}
						else
						{
							
							LookRight = false;
						}

						ActionAssistComp->OwnerParallelToCamera(LookRight);
					}
					else if (ActorHasTag("pattern2"))
					{
						if (bIsRotationTag2)
						{
						
							LookRight = true;

						}
						else
						{
							LookRight = false;
						}

						ActionAssistComp->OwnerParallelToCamera(LookRight);
					}
					else if (ActorHasTag("pattern3"))
					{
						if (bIsRotation)
						{
					
							LookRight = false;

						}
						else
						{

							LookRight = true;
						}

						ActionAssistComp->OwnerParallelToCamera(LookRight);
					}
				}

			}
		}
	}

	if (bHasPattern1Tag && GetWorld()->GetTimeSeconds() - SpawnTime < 4.5f || bHasPattern2Tag || bHasPattern3Tag)
	{
		float TimeSinceSpawn = GetWorld()->GetTimeSeconds() - SpawnTime;
		float SplineDuration = 3.0f;  // �X�v���C������������܂ł̎���
		float SplineProgress = FMath::Clamp(TimeSinceSpawn / SplineDuration, 0.0f, 1.0f);
		FVector NewLocation = MoveSpline->GetLocationAtSplineInputKey(SplineProgress, ESplineCoordinateSpace::World);
		SetActorLocation(NewLocation);
		for (AGameSystem_BattleArea* BattleArea : BattleAreaReferences) {
			if (BattleArea && BattleArea->IsOverlappingActor(this)) {
				BattleArea->IgnoreCollision();
			}
		}
		return; // ����Tick�������X�L�b�v
	}
	else
	{
		for (AGameSystem_BattleArea* BattleArea : BattleAreaReferences) {
			if (BattleArea) {
				BattleArea->ResetCollision();
			}
		}
	}
	if (bShouldMoveAlongSpline) {
		// 3�b�Ԉړ�������
		TimeSinceStartOfMovement += DeltaTime;
		float Alpha = FMath::Min(TimeSinceStartOfMovement / MovementDuration, 1.0f);
		FVector NewLocation = MovementSpline->GetLocationAtDistanceAlongSpline(MovementSpline->GetSplineLength() * Alpha, ESplineCoordinateSpace::Local);
		SetActorLocation(NewLocation);

		if (Alpha >= 1.0f) {
			bShouldMoveAlongSpline = false;
		}

		// ����Tick�������~����
		return;
	}

	CheckHealth();
	
}



/*
 * �֐����@�@�@�@�FAEnemyBase::ApplyDamage
 * �������e�@�@�@�F�_���[�W��K�p���A�L�����N�^�[�̏�Ԃ��X�V���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::TakedDamage(float DamageAmount, bool _bLastAttack)
{

	Health -= DamageAmount;
	FTimerHandle TimerHandle_ResetDamage;
	bIsNowDamage = true;
	GetWorldTimerManager().SetTimer(TimerHandle_ResetDamage, this, &AEnamyBase::DamageReac, 0.2f, false);
	ActionAssistComp->SpawnEffect(NiagaraEffect, GetActorLocation());
	HitStop(0.05f,0.1f);

	if (Health <= 0&& bGetAway == false)
	{
		PlayAnimMontage(Death, 0.8, NAME_None);

		GetWorldTimerManager().ClearTimer(TimerHandle_DestroyEnemy);
		GetWorldTimerManager().SetTimer(TimerHandle_DestroyEnemy, this, &AEnamyBase::AfterDeath, 1.0f, true);
		bIsDeath = true;
		FVector ForceDirection = -GetActorForwardVector();
		float ForceStrength = 80000.0f;  // Adjust this value as needed.

		GetCharacterMovement()->AddImpulse(ForceDirection * ForceStrength);


	}
	else
	{
		FVector ForceDirection = -GetActorForwardVector();
		float ForceStrength = 1000.0f;  // Adjust this value as needed.
		GetCharacterMovement()->AddImpulse(ForceDirection * ForceStrength);

		PlayAnimMontage(Damage_Reaction, 0.8, NAME_None);


	}


}


void AEnamyBase::Attack_Sword(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation)
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
				Cast<ACharacterBase>(ActorArray)->TakedDamage(10.f, false);
				break;
			}
		}
	}
}

void AEnamyBase::Attack_Lance(TTuple<TArray<AActor*>, TArray<FVector>> HitActorAndLocation)
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
				Cast<ACharacterBase>(ActorArray)->TakedDamage(10.f, false);
				break;
			}
		}
	}
}



/*
 * �֐����@�@�@�@�FAEnemyBase::MaintainDistanceFromEnemy
 * �������e�@�@�@�F�G�L�����N�^�[������̋�����ۂ悤�Ɉړ����܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::MaintainDistanceFromEnemy()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* CurrentActor = *It;
		if (CurrentActor && CurrentActor->ActorHasTag(FName("Enemy")))
		{
			FVector Direction = CurrentActor->GetActorLocation() - GetActorLocation();
			float Distance = Direction.Size();

			if (Distance < DesiredDistanceFromEnemy)
			{
				FVector MoveDirection = -Direction.GetSafeNormal() * (DesiredDistanceFromEnemy - Distance);
				AddMovementInput(MoveDirection);
			}
		}
	}
}
/*
 * �֐����@�@�@�@�FAEnemyBase::AfterDeath
 * �������e�@�@�@�F�L�����N�^�[�̎���̏������s���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::AfterDeath()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	DestoryEnemy();

}
/*
 * �֐����@�@�@�@�FAEnemyBase::DamageReac
 * �������e�@�@�@�F�_���[�W�����̃t���O�����Z�b�g���܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::DamageReac()
{
	bIsNowDamage = false;
}
/*
 * �֐����@�@�@�@�FAEnemyBase::CheckHealth
 * �������e�@�@�@�F�L�����N�^�[�̌��N��Ԃ��m�F���A�K�v�ɉ����Ď��S�g���K�[�𐶐����܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::CheckHealth()
{
	if (Health <= 0)
	{
		SpawnDeathTrigger();
	}
}
/*
 * �֐����@�@�@�@�FAEnemyBase::SpawnDeathTrigger
 * �������e�@�@�@�F���S�g���K�[�𐶐����܂��B
 * �߂�l�@�@�@�@�F�Ȃ��ivoid�j
 */
void AEnamyBase::SpawnDeathTrigger()
{
	if (DeathTriggerClass = nullptr)
	{
		FTransform SpawnTransform = GetActorTransform();
		ADeathTrigger* DeathTrigger = GetWorld()->SpawnActor<ADeathTrigger>(DeathTriggerClass, SpawnTransform);
		if (DeathTriggerClass != nullptr)
		{
			DeathTrigger->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}


/*
 * �֐����@�@�@�@�FAEnemyBase::GetEnemyActor
 * �������e�@�@�@�FEnemy�^�O�����A�N�^�[���擾���܂��B
 * �߂�l�@�@�@�@�FEnemy�^�O�����ŏ��̃A�N�^�[�B������Ȃ��ꍇ�� nullptr�B
 */
AActor* AEnamyBase::GetEnemyActor() const
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->ActorHasTag(FName("Enemy")))
		{
			return Actor; // Enemy�^�O�����ŏ��̃A�N�^�[��Ԃ�
		}
	}
	return nullptr; // Enemy�^�O�����A�N�^�[��������Ȃ��ꍇ
}

void AEnamyBase::OnOverlapBeginWall(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetCapsuleComponent())  // WallCollision �Ɠ���
	{
		// ���� EnemyBase �̃R���W�������X�|���X�� Block �ɐݒ�
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}
}




