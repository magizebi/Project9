// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberUtil.h"
#include "NumberPlayerController.h"
#include "Net/UnrealNetwork.h"

void UNumberUtil::MakeRandomValue(TArray<int32>& SomeArray)
{
    for (int32& Value : SomeArray)
    {
        Value = FMath::RandRange(0, 9);
    }
}

FString UNumberUtil::ArrayToString(const TArray<int32>& IntArray)
{
    TArray<FString> StringArray;

    for (int32 Value : IntArray)
    {
        StringArray.Add(FString::FromInt(Value));
    }

    return FString::Join(StringArray, TEXT(""));
}

float UNumberUtil::GetMouseX(ANumberPlayerController* Controller)
{
    float MouseX, MouseY;
    Controller->GetMousePosition(MouseX, MouseY);

    return MouseX;
}

UActorComponent* UNumberUtil::GetComponentUnderCursor(ANumberPlayerController* Controller)
{
    FHitResult HitResult;
    Controller->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

    UActorComponent* Component = HitResult.GetComponent();

    return Component;
}

void UNumberUtil::RemoveReplicatedWarning(TArray<FLifetimeProperty>& OutLifetimeProps)
{
    DISABLE_REPLICATED_PRIVATE_PROPERTY(UActorComponent, bReplicates);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(UActorComponent, bIsActive);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, RelativeLocation);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, RelativeRotation);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, RelativeScale3D);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bVisible);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, AttachParent);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, AttachChildren);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, AttachSocketName);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bAbsoluteLocation);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bAbsoluteRotation);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bAbsoluteScale);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bShouldBeAttached);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bShouldSnapLocationWhenAttached);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bShouldSnapRotationWhenAttached);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, bShouldSnapScaleWhenAttached);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(USceneComponent, Mobility);
    DISABLE_REPLICATED_PRIVATE_PROPERTY(UStaticMeshComponent, StaticMesh);
}
