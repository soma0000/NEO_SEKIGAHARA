// Fill out your copyright notice in the Description page of Project Settings.


#include "Destroyer.h"
#include "Components/BoxComponent.h"
#include "NEO/BackGroundSystem/ObjectBase.h"

// Sets default values
ADestroyer::ADestroyer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = BoxCollision;

	// スタティックメッシュコンポーネント作成
	Wood = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wood"));
	Wood->SetupAttachment(RootComponent);

	Human1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Human1"));
	Human1->SetupAttachment(RootComponent);

	Human2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Human2"));
	Human2->SetupAttachment(RootComponent);

	Human3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Human3"));
	Human3->SetupAttachment(RootComponent);

	Human4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Human4"));
	Human4->SetupAttachment(RootComponent);

	Human5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Human5"));
	Human5->SetupAttachment(RootComponent);

	Human6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Human6"));
	Human6->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADestroyer::BeginPlay()
{
	Super::BeginPlay();
	
}
