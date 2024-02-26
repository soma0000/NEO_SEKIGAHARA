// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "NEO/AnimationSystem/CharacterAnimInstance.h"
#include "Gun.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
	:bIsSpawnWeapon_BeginPlay(true)
{
	// Tick���񂷂��ǂ���
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// �Q�[���J�n���ɕ�����X�|�[��
	SpawnWeapon_BeginPlay();
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


/*
 * �֐����@�@�@�@�FAttachWeapon()
 * �������e�@�@�@�F����𑕔�����
 * �����P�@�@�@�@�FAWeaponBase* _newWeapon�E�E�E�擾���镐��
 * �����Q		 �FFName _socketName�E�E�E�E�E�E������������邽�߂̃\�P�b�g�̖��O
 * �����R�@�@�@�@�Fbool _bDestroy �E�E�E�E�E�E�E����������Ă���ꍇ�����Ɠ����ɍ폜���邩
 * �߂�l�@�@�@�@�F����������������
 */
bool UWeaponComponent::AttachWeapon(AWeaponBase* _newWeapon, FName _socketName /*= "None"*/, bool _bDestroy /*= true*/)
{
	// �V�������킪���݂��邩�\�P�b�g���ݒ肳��Ă��邩
	if (!_newWeapon || SocketNames.Num() <= 0) { return false; }

	// ���킪�擾�ł����Ԃ��m�F
	if (!(_newWeapon->GetIsPickUp())) { return false; }

	// ���̕���������Ă���Ƃ�����𗣂�
	if (Weapon) { DetachWeapon(_bDestroy);}

	// ������X�V
	Weapon = _newWeapon;

	// AnimationInstance�ɕ�����𑗂�
	SetWeaponType_AnimInstance();

	// �V�����\�P�b�g���w�肳�ꂽ��X�V����
	const FName NewSocketName = (_socketName != "None") ? (_socketName) : (SocketNames[0]);
	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());

	Weapon->AttachToCharacter(OwnerChara, NewSocketName);

	return true;
}


/*
 * �֐����@�@�@�@�FDetachWeapon()
 * �������e�@�@�@�F���������I�u�W�F�N�g��Actor�ɕϊ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UWeaponComponent::DetachWeapon(bool _bDestroy /*= true*/)
{
	if (!Weapon) { return; }

	// �����Ă��镐��𗣂����폜����
	(!_bDestroy) ? (Weapon->DetachFromCharacter()) : (Weapon->Destroy());

	// ����������Ă��Ȃ���Ԃ�
	Weapon = nullptr;

	// AnimationInstance�ɕ�����𑗂�
	SetWeaponType_AnimInstance();
}


/*
 * �֐����@�@�@�@�FChangeWeapon()
 * �������e�@�@�@�F�V��������𑕔�����
 * �����P�@�@�@�@�FTSubclassOf<AWeaponBase> _newWeaponClass�E�E�E�V��������̃N���X
 * �����Q		 �FFName _socketName �E�E�E�E�E�E�E�E�E�E�E�E�E�E�\�P�b�g�̖��O
 * �����R�@�@�@�@�Fbool _bDestroy�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�����ւ���O�̕�����폜���邩�ǂ���
 * �߂�l�@�@�@�@�F����������������
 */
bool UWeaponComponent::ChangeWeapon(TSubclassOf<AWeaponBase> _newWeaponClass, FName _socketName /*= "None"*/, bool _bDestroy /*= true*/)
{
	// �V�������킪�ݒ肳��Ă��邩�m�F
	if (!_newWeaponClass) { return false; }

	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerChara;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ����������Ă���ꍇ�藣��
	if (Weapon) { Weapon->DetachFromCharacter(); }

	// �������������邩�̃t���O
	bool bSucces = false;

	// �v���C���[�ɃA�^�b�`
	if (Weapon = GetWorld()->SpawnActor<AWeaponBase>(_newWeaponClass, SpawnParams))
	{
		Weapon->AttachToCharacter(OwnerChara, _socketName);

		bSucces = true;
	}

	return bSucces;
}


/*
 * �֐����@�@�@�@�FSetCollision()
 * �������e�@�@�@�F���������I�u�W�F�N�g��Actor�ɕϊ�
 * �߂�l�@�@�@�@�F�ϊ�����Actor�Ƃ��̍��W(�^�O�������Ă��Ȃ�Actor�͏Ȃ�)�̃^�v��
 */
TTuple<TArray<AActor*>, TArray<FVector>> UWeaponComponent::SetCollision()
{
	if (!Weapon) 
	{
		TArray<AActor*> NullActor = { nullptr };
		TArray<FVector> ZeroVector = { FVector::ZeroVector };
		return MakeTuple(NullActor, ZeroVector);
	}

	// Actor�Ƃ��̍��W�i�[�p
	TArray<AActor*> HitActors;
	TArray<FVector> HitLocations;

	if (Weapon->GetWeaponType() == EWeaponType::Gun)
	{
		// �ˌ�����
		Cast<AGun>(Weapon)->Shoot();
		TArray<AActor*> NullActor = { nullptr };
		TArray<FVector> ZeroVector = { FVector::ZeroVector };
	}
	else
	{
		// ����ɓ��������I�u�W�F�N�g�擾
		TArray<FHitResult> OutHitResults;
		Weapon->GetHitResults(OutHitResults);

		for (const FHitResult HitResult : OutHitResults)
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
	}

	return MakeTuple(HitActors, HitLocations);
}


/*
 * �֐����@�@�@�@�FSpawnWeapon_BeginPlay()
 * �������e�@�@�@�F�Q�[���J�n���ɕ�����o��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UWeaponComponent::SpawnWeapon_BeginPlay()
{
	// �Q�[���J�n���ɃX�|�[�����Ȃ��ݒ�̎�����������(BP����ݒ�\)���ݒ肳��Ă��Ȃ�������X���[
	if (!bIsSpawnWeapon_BeginPlay || !WeaponClass) { return; }

	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerChara;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ������X�|�[�����ăL�����N�^�[�ɃA�^�b�`
	if (Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams))
	{
		// AnimationInstance�ɕ�����𑗂�
		SetWeaponType_AnimInstance();

		// �\�P�b�g���ݒ肳��Ă��邩����
		if (SocketNames.Num() > 0) { Weapon->AttachToCharacter(OwnerChara, SocketNames[0]);}
	}
}


/*
 * �֐����@�@�@�@�FSetWeaponType_AnimInstance()
 * �������e�@�@�@�FAnimationInstance�ɕ�������Z�b�g
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UWeaponComponent::SetWeaponType_AnimInstance()
{
	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());
	if (!OwnerChara) { return; }
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(OwnerChara->GetMesh()->GetAnimInstance());
	if (!AnimInstance) { return; }

	// AnimationInstance�ɕ�����𑗂�
	EWeaponType WeaponType = (!Weapon) ? (EWeaponType::None) : (GetWeaponType());
	AnimInstance->SetWeaponType(WeaponType);
}
