// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "NEO/GameSystem/NEOGameMode.h"
#include "NEO/CharacterSystem/PlayerSystem/NEOPlayerController.h"


// Sets default values
ASplineCamera::ASplineCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraSpringArm);

	//CreateSplineComponent
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));

	CameraSpeed = 300.0f;
	CurrentRatio = 0;

}


void ASplineCamera::SetPlayerCamera()
{
	//SetViewTarget
	ANEOGameMode* pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (pGameMode)
	{
		pGameMode->SetPlayerCamera(this);
		pGameMode->SetViewTargetWithBlend(this);
	}

	// プレイヤーコントローラーを取得
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Called when the game starts or when spawned
void ASplineCamera::BeginPlay()
{
	Super::BeginPlay();

	if (SplineComponent) {
		FVector CameraLocation = SplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);

		CameraComponent->SetWorldLocation(CameraLocation);
	}
}

// Called every frame
void ASplineCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetCamera();
}


void ASplineCamera::SetCamera()
{
	if (PlayerController && SplineComponent)
	{
		// Spline曲線上でプレイヤーに最も近い点を取得
		FVector NearestPoint = SplineComponent->FindLocationClosestToWorldLocation(PlayerController->GetPlayerLocation(), ESplineCoordinateSpace::World);


		FVector BeforeCameraLocation = CameraComponent->GetComponentLocation();		//カメラの座標を取得

		FVector CameraLocation = FMath::Lerp(BeforeCameraLocation, NearestPoint, 0.08);

		CameraComponent->SetWorldLocation(CameraLocation);

		//Spline曲線上でプレイヤーに最も近い点の回転を取得
		FRotator NewRotation = SplineComponent->FindRotationClosestToWorldLocation(CameraLocation, ESplineCoordinateSpace::World);

		//プレイヤーの方を向くように調整
		NewRotation.Yaw += -90.f;
		NewRotation.Pitch = -25.f;

		CameraComponent->SetWorldRotation(NewRotation);
	}
}