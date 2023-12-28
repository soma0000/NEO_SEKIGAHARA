// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Lance.generated.h"

UCLASS()
class NEO_API ALance : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
