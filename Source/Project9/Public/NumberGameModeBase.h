// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NumberTypes.h"
#include "NumberGameModeBase.generated.h"

UCLASS()
class PROJECT9_API ANumberGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

    // === Constructor ===
public:
    ANumberGameModeBase();

    // === Event Methods ===
public:
    virtual void BeginPlay() override;
    // === Member Variables ===
private:
    class ANumberGameStateBase* ServerGameState;
    TArray<int32> Password;

    // === Member Functions ===
public:
    void HandleJudgement();
private:
    TArray<EHitType> GetPasswordResult(const TArray<int32>& Guess);
    bool GetVictoryCondition(const TArray<EHitType>& Result) const;
    void NextTurn();
    void StartGame();
    void EndGame();
    void SelectFirstTurnPlayer();
    void SelectNextPlayer();
    void CheckWorldLock();
    void UpdateLockOwner();
};
