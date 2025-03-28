// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberGameStateBase.h"
#include "NumberGameModeBase.h"
#include "NumberplayerState.h"
#include "NumberHUD.h"
#include "MainWidget.h"
#include "LEDLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CombinationLock.h"
#include "Net/UnrealNetwork.h"

ANumberGameStateBase::ANumberGameStateBase()
{
    bReplicates = true;

    LockActor = nullptr;
    GameData.CurrentTurnPlayer = nullptr;
    GameData.CurrentResult.SetNum(4);
    GameData.PreviousPassword.SetNum(4);
    GameData.TurnCount = 0;
}

void ANumberGameStateBase::BeginPlay()
{
    Super::BeginPlay();

    LockActor = Cast<ACombinationLock>(UGameplayStatics::GetActorOfClass(GetWorld(), ACombinationLock::StaticClass()));

    if (!HasAuthority()) return;

    LockActor->Muticast_UpdateShackle(true);
}

const FNumberGameData& ANumberGameStateBase::GetGameData() const
{
    return GameData;
}

ACombinationLock* ANumberGameStateBase::GetLock()
{
    return LockActor;
}

ANumberPlayerState* ANumberGameStateBase::GetCurrentTurnPlayer() const
{
    return GameData.CurrentTurnPlayer;
}

void ANumberGameStateBase::UpdateLockLightColor()
{
    for (int32 Index = 0; Index < 4; Index++)
    {
        ULEDLightComponent* LEDLight = LockActor->GetLight(Index);
        if (!LEDLight) return;

        switch (GameData.CurrentResult[Index])
        {
        case EHitType::Strike:
            LEDLight->Multicast_SetColor(FLinearColor::Green);
            break;
        case EHitType::Ball:
            LEDLight->Multicast_SetColor(FLinearColor::Yellow);
            break;
        case EHitType::None:
            LEDLight->Multicast_SetColor(FLinearColor::Red);
            break;
        }
    }
}

void ANumberGameStateBase::RequestJudgement()
{
    if (!HasAuthority()) return;

    ANumberGameModeBase* NumberGameMode = GetWorld()->GetAuthGameMode<ANumberGameModeBase>();
    if (!NumberGameMode) return;

    NumberGameMode->HandleJudgement();
    UpdateLockLightColor();
    OnRep_GameData();
}

void ANumberGameStateBase::OnRep_GameData()
{
    ANumberHUD* HUD = ANumberHUD::GetHUD(this);
    if (!HUD) return;

    HUD->UpdateMainWidget(GameData);
}

void ANumberGameStateBase::OnRep_Winner()
{
}

void ANumberGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ANumberGameStateBase, LockActor);
    DOREPLIFETIME(ANumberGameStateBase, GameData);
    DOREPLIFETIME(ANumberGameStateBase, Winner);
}
