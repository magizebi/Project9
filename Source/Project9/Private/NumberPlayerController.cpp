// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberPlayerController.h"
#include "NumberGameStateBase.h"
#include "NumberPlayerState.h"
#include "NumberUtil.h"
#include "NumberHUD.h"
#include "CombinationLock.h"
#include "DialComponent.h"
#include "InteractableInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ANumberPlayerController::ANumberPlayerController()
{
    InputMappingContext = nullptr;
    DragAction = nullptr;
    ClickAction = nullptr;
}

void ANumberPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Enhanced InputComponent 사용
    UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);

    if (!EnhancedInputComp) return;
        // 마우스 X 입력 바인딩
    EnhancedInputComp->BindAction(ClickAction, ETriggerEvent::Started, this, &ANumberPlayerController::StartInteract);
    EnhancedInputComp->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ANumberPlayerController::Interact);
    EnhancedInputComp->BindAction(ClickAction, ETriggerEvent::Completed, this, &ANumberPlayerController::EndInteract);
    
}

void ANumberPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    // 인풋 매핑
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!SubSystem || !InputMappingContext) return;

    SubSystem->AddMappingContext(InputMappingContext, 0);

    // 카메라 선택
    ANumberGameStateBase* NumberGameState = GetWorld()->GetGameState<ANumberGameStateBase>();
    if (!NumberGameState) return;

    ACombinationLock* Lock = NumberGameState->GetLock();
    if (!Lock) return;

    SetViewTargetWithBlend(Lock);

    // 개인 UI 초기화
    if (!HasAuthority()) return;
    GetWorld()->GetTimerManager().SetTimerForNextTick([this, NumberGameState]()
    {
        ANumberHUD* HUD = ANumberHUD::GetHUD(this);
        if (!HUD) return;

        HUD->InitUI();
    });
}

void ANumberPlayerController::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    ANumberHUD* HUD = ANumberHUD::GetHUD(this);
    if (!HUD) return;

    HUD->InitUI();
}

void ANumberPlayerController::StartInteract(const FInputActionValue& Value)
{
    // 자기 턴일 때만 조작 가능
    ANumberPlayerState* NumberPlayer = GetPlayerState<ANumberPlayerState>();
    if (!NumberPlayer || !NumberPlayer->IsMyTurn()) return;

    // 마우스 아래에 있는 액터를 찾기
    UActorComponent* Component = UNumberUtil::GetComponentUnderCursor(this);

    if (Component && Component->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
    {
        SelectedComponent = Cast<IInteractableInterface>(Component);
    }

    if (!SelectedComponent) return;
    
    SelectedComponent->StartInteract(this);
}

void ANumberPlayerController::Interact(const FInputActionValue& Value)
{
    if (!SelectedComponent) return;

    SelectedComponent->Interact(this);
}

void ANumberPlayerController::EndInteract(const FInputActionValue& Value)
{
    if (!SelectedComponent)  return;

    SelectedComponent->EndInteract(this);
    SelectedComponent = nullptr;
}
