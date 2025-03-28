// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "NumberTypes.h"
#include "LEDLightComponent.generated.h"

UCLASS()
class PROJECT9_API ULEDLightComponent : public UStaticMeshComponent
{
    GENERATED_BODY()

    // === Constructor ===
public:
    ULEDLightComponent();

    // === Event Functions ===
protected:
    virtual void OnComponentCreated() override;
    virtual void OnRegister() override;
    virtual void BeginPlay() override;

    // === Member Variables ===
protected:
    UMaterialInstanceDynamic* DynamicMaterial;

    bool bIsTurnOn;
    float Brightness;
    FLinearColor CurrentColor;

    // === Member Functions ===
public:
    UFUNCTION(BlueprintCallable)
    void SetBrightness(float Value);

    UFUNCTION(BlueprintCallable)
    void SetSwitch(bool Value);

    UFUNCTION(BlueprintCallable)
    void SetColor(FLinearColor Color);

protected:
    void UpdateLED();

    // === Server Functions ===
public:
    UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
    void Multicast_SetColor(FLinearColor Color);
};
