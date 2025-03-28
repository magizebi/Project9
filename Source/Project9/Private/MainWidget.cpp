// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/TextBlock.h"

FName UMainWidget::GetWidgetNameFromLabel(ETextBlockName TextBlockName) const
{
    switch (TextBlockName)
    {
    case ETextBlockName::MyName:            return FName("MyName");
    case ETextBlockName::CurrentPlayer:     return FName("CurrentPlayer");
    case ETextBlockName::PreviousValue:     return FName("PreviousValue");
    case ETextBlockName::PreviousResult:    return FName("PreviousResult");
    case ETextBlockName::CurrentValue:      return FName("CurrentValue");
    case ETextBlockName::TurnCount:         return FName("TurnCount");
    case ETextBlockName::StrikeCount:       return FName("StrikeCount");
    case ETextBlockName::BallCount:         return FName("BallCount");
    default:                                return FName("");
    }
}

void UMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    for (uint8 i = 0; i < static_cast<uint8>(ETextBlockName::BallCount) + 1; i++)
    {
        ETextBlockName TextBlockName = static_cast<ETextBlockName>(i);
        FName WidgetName = GetWidgetNameFromLabel(TextBlockName);

        if (UTextBlock* TextWidget = Cast<UTextBlock>(GetWidgetFromName(WidgetName)))
        {
            TextBockNameMap.Add(TextBlockName, TextWidget);
        }
    }
}

void UMainWidget::SetLabelText(ETextBlockName TextBlockName, const FString& Text)
{
    if (UTextBlock* TextBlock = TextBockNameMap.FindRef(TextBlockName))
    {
        TextBlock->SetText(FText::FromString(Text));
    }
}
