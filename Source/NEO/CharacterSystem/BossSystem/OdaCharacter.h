// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OdaBase.h"
#include "OdaCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API AOdaCharacter : public AOdaBase
{
	GENERATED_BODY()

		//�K�E�Z���̏Ռ��g�̃_���[�W
		float UltShockWaveDamage;

	//�K�E�Z�̃_���[�W
		float UltDamage;

	void OdaUlt(int Timer);
};
