// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destroyer.generated.h"

UCLASS()
class NEO_API ADestroyer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestroyer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Collision")
		TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> Wood;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> Human1;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> Human2;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> Human3;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> Human4;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> Human5;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> Human6;
};
