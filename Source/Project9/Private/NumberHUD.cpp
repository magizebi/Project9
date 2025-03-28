// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberHUD.h"
#include "NumberGameStateBase.h"
#include "NumberPlayerController.h"
#include "NumberPlayerState.h"
#include "NumberUtil.h"

void ANumberHUD::BeginPlay()
{
    Super::BeginPlay();

    if (!MainWidgetClass) return;
    
    MainWidget = Cast<UMainWidget>(CreateWidget(GetWorld(), MainWidgetClass));
    if (MainWidget)
    {
        MainWidget->AddToViewport();
    }
    
}

ANumberHUD* ANumberHUD::GetHUD(const UObject* WorldContext)
{
    if (!WorldContext) return nullptr;

    UWorld* World = WorldContext->GetWorld();
    if (!World) return nullptr;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC) return nullptr;

    return Cast<ANumberHUD>(PC->GetHUD());
}

UMainWidget* ANumberHUD::GetMainWidget() const
{
    return MainWidget;
}

void ANumberHUD::UpdateMainWidget(const FNumberGameData& Data)
{
    if (!MainWidget) return;
    if (!Data.CurrentTurnPlayer) return;

    int32 Strike = 0, Ball = 0;
    for (EHitType Hit : Data.CurrentResult)
    {
        if (Hit == EHitType::Strike)
        {
            Strike++;
        }
        else if (Hit == EHitType::Ball)
        {
            Ball++;
        }
    }

    MainWidget->SetLabelText(ETextBlockName::StrikeCount, FString::FromInt(Strike));
    MainWidget->SetLabelText(ETextBlockName::BallCount, FString::FromInt(Ball));
    MainWidget->SetLabelText(ETextBlockName::CurrentPlayer, Data.CurrentTurnPlayer->GetName());
    MainWidget->SetLabelText(ETextBlockName::TurnCount, FString::FromInt(Data.TurnCount));
    MainWidget->SetLabelText(ETextBlockName::PreviousValue, UNumberUtil::ArrayToString(Data.PreviousPassword));
}

void ANumberHUD::UpdateCurrentPassword(TArray<int32> Password)
{
    if (!MainWidget) return;

    MainWidget->SetLabelText(ETextBlockName::CurrentValue, UNumberUtil::ArrayToString(Password));
}

void ANumberHUD::UpdateMyName(const FString& PlayerName)
{
    if (!MainWidget) return;

    MainWidget->SetLabelText(ETextBlockName::MyName, PlayerName);
}

void ANumberHUD::InitUI()
{
    APlayerController* PC = GetOwningPlayerController();
    ANumberPlayerState* PS = PC ? PC->GetPlayerState<ANumberPlayerState>() : nullptr;
    ANumberGameStateBase* GS = GetWorld()->GetGameState<ANumberGameStateBase>();

    if (!MainWidget || !PS || !GS) return;
    if (!GS->GetGameData().CurrentTurnPlayer) return;

    UpdateMyName(PS->GetName());
    UpdateMainWidget(GS->GetGameData());
}
