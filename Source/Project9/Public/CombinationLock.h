// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombinationLock.generated.h"

UCLASS()
class PROJECT9_API ACombinationLock : public AActor
{
    GENERATED_BODY()

    // === Constructor ===
public:
    ACombinationLock();

    // === Event Methods ===
protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    virtual void Tick(float DeltaTime) override;

    // === Member Variables ===
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock|Components")
    class USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock|Components")
    class UCameraComponent* LockCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock|Components")
    UStaticMeshComponent* LockBody;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock|Components")
    UStaticMeshComponent* Shackle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock|Components")
    TArray<class UDialComponent*> Dials;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock|Components")
    TArray<class ULEDLightComponent*> Lights;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lock|Components")
    class UButtonComponent* Button;

    float ShackleLocation;
    float ShackleHeight;

    // === Member Methods ===
public:
    UFUNCTION(BlueprintCallable)
    UCameraComponent* GetCamera() const;

    UFUNCTION(BlueprintCallable)
    UDialComponent* GetDial(int32 Index) const;

    UFUNCTION(BlueprintCallable)
    ULEDLightComponent* GetLight(int32 Index) const;

    UFUNCTION(BlueprintCallable)
    int32 GetDialAmount() const;

    UFUNCTION(BlueprintCallable)
    TArray<int32> GetPassword() const;
private:
    void AttachComponentsToSocket(USceneComponent* Component, FName SocketName);
    void AttachComponentsToSockets();

    // === Server Methods ===
public:
    UFUNCTION(NetMulticast,Reliable, BlueprintCallable)
    void Muticast_UpdateShackle(bool Value);
};
