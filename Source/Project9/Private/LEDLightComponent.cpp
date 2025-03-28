// Fill out your copyright notice in the Description page of Project Settings.


#include "LEDLightComponent.h"
#include "Net/UnrealNetwork.h"

ULEDLightComponent::ULEDLightComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicated(true);

    DynamicMaterial = nullptr;
    CurrentColor = FLinearColor::Black;
    bIsTurnOn = false;
    Brightness = 0.f;
}

void ULEDLightComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

}

void ULEDLightComponent::OnRegister()
{
    Super::OnRegister();

    DynamicMaterial = CreateDynamicMaterialInstance(0);
    
}

void ULEDLightComponent::BeginPlay()
{
    Super::BeginPlay();

    UpdateLED();
}

void ULEDLightComponent::SetBrightness(float Value)
{
    Brightness = Value;

    UpdateLED();
}

void ULEDLightComponent::SetSwitch(bool Value)
{
    bIsTurnOn = Value;

    UpdateLED();
}

void ULEDLightComponent::SetColor(FLinearColor Color)
{
    CurrentColor = Color;

    UpdateLED();
}

void ULEDLightComponent::UpdateLED()
{
    if (!DynamicMaterial) return;
    
    float Power = bIsTurnOn ? Brightness : 0.f;

    DynamicMaterial->SetVectorParameterValue("LEDColor", CurrentColor);
    DynamicMaterial->SetScalarParameterValue("LightPower", Power);
    
}
void ULEDLightComponent::Multicast_SetColor_Implementation(FLinearColor Color)
{
    if (!bIsTurnOn)
    {
        SetSwitch(true);
    }
    SetColor(Color);

    UpdateLED();
}
