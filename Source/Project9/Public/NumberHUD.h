// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainWidget.h"
#include "NumberTypes.h"
#include "NumberHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT9_API ANumberHUD : public AHUD
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainWidgetClass;

    UPROPERTY()
    class UMainWidget* MainWidget;

public:
    static ANumberHUD* GetHUD(const UObject* WorldContext);

    UMainWidget* GetMainWidget() const;
    void UpdateMainWidget(const FNumberGameData& Data);
    void UpdateCurrentPassword(TArray<int32> Password);
    void UpdateMyName(const FString& PlayerName);
    void InitUI();
};
