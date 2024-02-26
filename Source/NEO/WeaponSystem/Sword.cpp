// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ����̃��b�V���̃p�X
	TCHAR* WeaponAssetPath = TEXT("/Game/0122/Player/Weapon/Sword/Sword");

	// ����̃��b�V���ݒ�
	SetupWeaponMesh(WeaponStaticMesh, WeaponAssetPath);

	// �R���W�����ݒ�
	SetupCollisionComponent(WeaponCollision);

	// ����̎�ނ𓁂ɐݒ�
	WeaponType = EWeaponType::Sword;
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}