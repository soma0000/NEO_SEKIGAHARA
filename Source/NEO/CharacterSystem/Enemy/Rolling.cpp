// Fill out your copyright notice in the Description page of Project Settings.


#include "Rolling.h"
#include "NEO/CharacterSystem/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ARolling::ARolling()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	RollingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RollingSphere"));
	RollingSphere->OnComponentBeginOverlap.AddDynamic(this, &ARolling::OnOverlap);
	RollingSphere->SetupAttachment(Sphere);
	Damage = 5.0f;
}

void ARolling::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor->Tags.Num() <= 0 || OtherActor->ActorHasTag("Pusher")) { return; }

	if (OtherActor->ActorHasTag("Player"))
	{
		ACharacterBase* CastedPlayer = Cast<ACharacterBase>(OtherActor);
		CastedPlayer->TakedDamage(Damage);
		Destroy();
	}
	else if(OtherActor->ActorHasTag("Enemy"))
	{
		ACharacterBase* CastedEnemy = Cast<ACharacterBase>(OtherActor);
		CastedEnemy->TakedDamage(Damage);
		Destroy();
	}
}

// Called when the game starts or when spawned
void ARolling::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(8.0f);
}

// Called every frame
void ARolling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARolling::DestoryRock()
{
	ARolling* RollingActor = nullptr;
	GetWorld()->DestroyActor(RollingActor);
}