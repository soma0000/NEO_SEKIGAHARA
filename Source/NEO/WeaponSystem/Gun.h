// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Gun.generated.h"

UCLASS()
class NEO_API AGun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	// “–‚½‚è”»’è
	void Shoot(int32 _damageAmount = 0);

private:

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APlayerBullet> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Sound")
		TObjectPtr<class USoundBase> ShootSoundObj;
};
