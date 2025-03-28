// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NumberPlayerController.generated.h"

UCLASS()
class PROJECT9_API ANumberPlayerController : public APlayerController
{
    GENERATED_BODY()

    // === Constructor ===
public:
    ANumberPlayerController();

    // === Event Functions ===
protected:
    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;
    virtual void OnRep_PlayerState() override;

    // === Member Variables ===
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputAction* ClickAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    class UInputAction* DragAction;
private:
    class IInteractableInterface* SelectedComponent;
    // === Input Functions ===
private:
    void StartInteract(const struct FInputActionValue& Value);
    void Interact(const FInputActionValue& Value);
    void EndInteract(const FInputActionValue& Value);
};
