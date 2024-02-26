// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, DebugModule);

// Sets default values for this component's properties
UDebugComponent::UDebugComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UDebugComponent::BeginPlay()
{
	Super::BeginPlay();

	// �v���C���[�R���g���[���[���擾
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DebugActions.DebugKeyMappingContext, 1);
		}
	}
}


/*
 * �֐����@�@�@�@�FSetupDebugInputComponent()
 * �������e�@�@�@�F���͂̃o�C���h
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UDebugComponent::SetupDebugInputComponent(UInputComponent* DebugInputComponent)
{

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(DebugInputComponent))
	{
		EnhancedInputComponent->BindAction(DebugActions.Debug_ESC, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_ESC);

		EnhancedInputComponent->BindAction(DebugActions.Debug_F1, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F1);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F4, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F4);

// �V�b�s���O���f�o�b�O�L�[���Ȃ�
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
		EnhancedInputComponent->BindAction(DebugActions.Debug_F2, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F2);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F3, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F3);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F5, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F5);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F6, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F6);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F7, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F7);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F8, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F8);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F9, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F9);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F10, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F10);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F11, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F11);
		EnhancedInputComponent->BindAction(DebugActions.Debug_F12, ETriggerEvent::Started, this, &UDebugComponent::TriggerDebugEvent_F12);

#endif

	}	

}
