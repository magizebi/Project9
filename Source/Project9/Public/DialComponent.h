// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableInterface.h"
#include "DialComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT9_API UDialComponent : public UStaticMeshComponent, public IInteractableInterface
{
    GENERATED_BODY()

    // === Constructor ===
public:
    UDialComponent();

    // === Member Variables ===
protected:
    float StartYaw;
    float StartMouseX;
    UPROPERTY(ReplicatedUsing = OnRep_Dialvalue)
    int32 DialValue;
    UPROPERTY(ReplicatedUsing = OnRep_DialRotation)
    int32 DialRotation;

    // === Member Methods ===
public:
    int32 GetDialValue() const;

    // === InteractableInterface Methods ===
protected:
    virtual void StartInteract(ANumberPlayerController* Controller) override;
    virtual void Interact(ANumberPlayerController* Controller) override;
    virtual void EndInteract(ANumberPlayerController* Controller) override;

    // === Server Methods ===
public:
    UFUNCTION(Server, Unreliable, BlueprintCallable)
    void Server_SetDialRotation(float Yaw);

    UFUNCTION(Server, Reliable, BlueprintCallable)
    void Server_SnapDial(const FRotator& Rotation);

    UFUNCTION()
    void OnRep_Dialvalue();
    UFUNCTION()
    void OnRep_DialRotation();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
