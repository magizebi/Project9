// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NumberUtil.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT9_API UNumberUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    // ===  Math Util ===
    static void MakeRandomValue(TArray<int32>& SomeArray);

    template<typename KeyType, typename ValueType>
    static ValueType& FindRef(TMap<KeyType, ValueType>& Map, const KeyType& Key, ValueType& DefaultValue)
    {
        if (ValueType* Ptr = Map.Find(Key))
        {
            return *Ptr;
        }
        return DefaultValue;
    }

    static FString ArrayToString(const TArray<int32>& IntArray);
    // === Controller Util ===
    static float GetMouseX(class ANumberPlayerController* Controller);
    static UActorComponent* GetComponentUnderCursor(ANumberPlayerController* Controller);
    // === Replicate Util ===
    static void RemoveReplicatedWarning(TArray<FLifetimeProperty>& OutLifetimeProps);
};
