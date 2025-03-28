// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableInterface.h"
#include "ButtonComponent.generated.h"

UCLASS()
class PROJECT9_API UButtonComponent : public UStaticMeshComponent, public IInteractableInterface
{
    GENERATED_BODY()

    // === Constructor ===
public:
    UButtonComponent();

    // === Event Functions ===
public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // === InteractableInterface Methods ===
protected:
    virtual void StartInteract(ANumberPlayerController* Controller) override;
    virtual void Interact(ANumberPlayerController* Controller) override;
    virtual void EndInteract(ANumberPlayerController* Controller) override;

    // === Member Variables ===
protected:
    UPROPERTY(ReplicatedUsing = OnRep_IsPressed)
    bool bIsPressed;

    float MovementValue;
    float StartLocation;

    // === Server Functions ===
public:
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void Server_UpdateButtonPressed(bool Value);

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void Server_EndTurn();

protected:
    UFUNCTION()
    void OnRep_IsPressed();
};
