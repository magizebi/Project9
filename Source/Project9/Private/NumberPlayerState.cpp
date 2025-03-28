// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberPlayerState.h"
#include "Net/UnrealNetwork.h"

ANumberPlayerState::ANumberPlayerState()
{
    bIsMyTurn = false;
}

void ANumberPlayerState::SetMyTurn(bool Value)
{
    bIsMyTurn = Value;
}

bool ANumberPlayerState::IsMyTurn() const
{
    return bIsMyTurn;
}

FString ANumberPlayerState::GetName() const
{
    return FString::Printf(TEXT("Player-%d"), GetPlayerId());
}

void ANumberPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ANumberPlayerState, bIsMyTurn);
}
