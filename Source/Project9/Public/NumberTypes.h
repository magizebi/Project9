#pragma once

#include "CoreMinimal.h"
#include "NumberTypes.generated.h"

UENUM(BlueprintType)
enum class EHitType : uint8
{
    None   UMETA(DisplayName = "None"),
    Ball   UMETA(DisplayName = "Ball"),
    Strike UMETA(DisplayName = "Strike")
};

USTRUCT(BlueprintType)
struct FNumberGameData
{
    GENERATED_BODY()

    UPROPERTY()
    class ANumberPlayerState* CurrentTurnPlayer;

    UPROPERTY()
    TArray<EHitType> CurrentResult;

    UPROPERTY()
    TArray<int32> PreviousPassword;

    UPROPERTY()
    int32 TurnCount;
};
