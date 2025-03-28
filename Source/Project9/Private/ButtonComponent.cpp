// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonComponent.h"
#include "NumberPlayerController.h"
#include "NumberGameStateBase.h"
#include "NumberUtil.h"
#include "Net/UnrealNetwork.h"

UButtonComponent::UButtonComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicated(true);

    bIsPressed = false;
    MovementValue = 5.f;
    StartLocation = GetComponentLocation().Z;
}

void UButtonComponent::StartInteract(ANumberPlayerController* Controller)
{
    Server_UpdateButtonPressed(true);
}

void UButtonComponent::Interact(ANumberPlayerController* Controller)
{
    if (!bIsPressed) return;

    UActorComponent* Component = UNumberUtil::GetComponentUnderCursor(Controller);
    if (Component && !Component->IsA<UButtonComponent>())
    {
        Server_UpdateButtonPressed(false);
    }
}

void UButtonComponent::EndInteract(ANumberPlayerController* Controller)
{
    if (!bIsPressed) return;

    Server_UpdateButtonPressed(false);
    Server_EndTurn();
}

void UButtonComponent::Server_EndTurn_Implementation()
{
    ANumberGameStateBase* NumberGameState = GetWorld()->GetGameState<ANumberGameStateBase>();
    if (!NumberGameState) return;

    NumberGameState->RequestJudgement();
}

void UButtonComponent::Server_UpdateButtonPressed_Implementation(bool Value)
{
    bIsPressed = Value;
    OnRep_IsPressed();
}
void UButtonComponent::OnRep_IsPressed()
{
    FVector Location = GetComponentLocation();
    Location.Z = bIsPressed ? StartLocation : StartLocation + MovementValue;
    SetWorldLocation(Location);
}
void UButtonComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(UButtonComponent, bIsPressed);

    UNumberUtil::RemoveReplicatedWarning(OutLifetimeProps);
}
