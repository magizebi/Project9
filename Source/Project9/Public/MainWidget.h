// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UTextBlock;

UENUM(BlueprintType)
enum class ETextBlockName : uint8
{
    MyName,
    CurrentPlayer,
    PreviousValue,
    PreviousResult,
    CurrentValue,
    TurnCount,
    StrikeCount,
    BallCount
};

UCLASS()
class PROJECT9_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetLabelText(ETextBlockName TextBlockName, const FString& Text);

protected:
    virtual void NativeConstruct() override;

    TMap<ETextBlockName, UTextBlock*> TextBockNameMap;

    FName GetWidgetNameFromLabel(ETextBlockName Label) const;
};
