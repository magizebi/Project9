// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NumberTypes.h"
#include "NumberGameStateBase.generated.h"

class ANumberPlayerState;
class ACombinationLock;

UCLASS()
class PROJECT9_API ANumberGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

    friend class ANumberGameModeBase;

    // === Constructor ===
public:
    ANumberGameStateBase();

    // === Event Functions ===
protected:
    virtual void BeginPlay() override;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // === Member Variables ===
private:
    UPROPERTY(Replicated)
    ACombinationLock* LockActor;

    UPROPERTY(ReplicatedUsing = OnRep_Winner)
    ANumberPlayerState* Winner;

    UPROPERTY(ReplicatedUsing = OnRep_GameData)
    FNumberGameData GameData;

    // === Member Functions ===
public:
    const FNumberGameData& GetGameData() const;
    ACombinationLock* GetLock();
    ANumberPlayerState* GetCurrentTurnPlayer() const;
    void RequestJudgement();

private:
    void UpdateLockLightColor();

    // === Replication Functions ===
public:
    UFUNCTION()
    void OnRep_GameData();

    UFUNCTION()
    void OnRep_Winner();
};
