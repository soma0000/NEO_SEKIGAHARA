// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"

#include "WeaponComponent.generated.h"

class AWeaponBase;

UCLASS( ClassGroup=(WeaponComponent), meta=(BlueprintSpawnableComponent) )
class NEO_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// ������擾
	bool AttachWeapon(AWeaponBase* _newWeapon, FName _socketName = "None", bool _bDestroy = true);

	// ����𗣂�
	void DetachWeapon(bool _bDestroy = true);

	// ����������ւ���
	bool ChangeWeapon(TSubclassOf<AWeaponBase> _newWeaponClass, FName _socketName = "None", bool _bDestroy = true);

	// ����擾
	AWeaponBase* GetWeapon()const { return Weapon; }

	// ����̍U���͎擾
	int32 GetWeaponDamage()const { return Weapon->GetDamage(); }

	FName GetSocketName(int32 _index)const { return SocketNames[_index]; }

	// ���݂̕���̎�ގ擾
	EWeaponType GetWeaponType()const { return Weapon->GetWeaponType(); }

	// �U�����蔭��
	TTuple<TArray<AActor*>, TArray<FVector>> SetCollision();

protected:

	// Sets default values for this component's properties
	UWeaponComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

private:

	// �Q�[���J�n���ɕ�����X�|�[��
	void SpawnWeapon_BeginPlay();

	// �A�j���[�V�����C���X�^���X�ɕ�������Z�b�g
	void SetWeaponType_AnimInstance();

protected:

	// ����̎w��
	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<AWeaponBase> WeaponClass;

	// ����̃\�P�b�g�̖��O
	UPROPERTY(EditAnywhere, Category = "Weapon")
		TArray<FName> SocketNames;

private:

	// �Q�[���J�n���ɕ�����������邩�ǂ���
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		bool bIsSpawnWeapon_BeginPlay;

	// ����N���X�̃|�C���^
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AWeaponBase> Weapon;
};
