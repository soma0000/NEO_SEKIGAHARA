// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "NEO/CharacterSystem/ActionAssistComponent.h"
#include "PlayerBullet.h"


// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ����̃��b�V���̃p�X
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Gun/Gun");

	// ����̃��b�V���ݒ�
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// �R���W�����ݒ�
	SetupCollisionComponent(WeaponCollision);

	// ����̎�ނ��e�ɐݒ�
	WeaponType = EWeaponType::WeaponType_Gun;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
 * �֐����@�@�@�@�FShoot()
 * �������e�@�@�@�F�ˌ�����
 * �����P�@�@�@�@�Fint32�E�E�E�e�ۂ̍U����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AGun::Shoot(int32 _damageAmount /*= 0*/)
{
	// �e�e���X�|�[��
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// �X�|�[�����̏ꏊ�Ɖ�]���w��
	FVector SpawnBulletLocation = GetActorLocation();

	FRotator SpawnBulletRotation = pOwner->GetActorRotation();

	// ����
	APlayerBullet* Bullet = GetWorld()->SpawnActor<APlayerBullet>(BulletClass, SpawnBulletLocation, SpawnBulletRotation, SpawnParams);

	// �V�����U���͂��ݒ肳��Ă��Ȃ���Ό��̍U���͂��Q��
	int32 DamageAmount = (_damageAmount != 0) ? (_damageAmount) : (GetDamage());

	// �U���͐ݒ�
	Bullet->SetDamageAmount(DamageAmount);

	if (ShootSoundObj)
	{
		// ���ˉ��Đ�
		ActionAssistComp->PlaySound(ShootSoundObj);
	}
}