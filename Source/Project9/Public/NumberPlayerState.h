// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NumberPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT9_API ANumberPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
    ANumberPlayerState();
protected:
    UPROPERTY(Replicated)
    bool bIsMyTurn;

public:
    void SetMyTurn(bool Value);
    bool IsMyTurn() const;
    FString GetName() const;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
