// Fill out your copyright notice in the Description page of Project Settings.


#include "DialComponent.h"
#include "CombinationLock.h"
#include "NumberPlayerController.h"
#include "NumberUtil.h"
#include "Net/UnrealNetwork.h"
#include "NumberHUD.h"

// Sets default values for this component's properties
UDialComponent::UDialComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicated(true);

    StartYaw = 0.f;
    StartMouseX = 0.f;
    DialValue = 0;
}

// 🔹 서버에서 다이얼 값 변경
void UDialComponent::Server_SetDialRotation_Implementation(float Yaw)
{
    FRotator NewRot = GetRelativeRotation();
    NewRot.Yaw = Yaw;
    SetRelativeRotation(NewRot);
    // 동기화 할 변수
    DialRotation = Yaw;
}

void UDialComponent::Server_SnapDial_Implementation(const FRotator& Rotation)
{
    float StepAngle = 36.f;
    float NormalYaw = FMath::Fmod(Rotation.Yaw, 360.f);
    if (NormalYaw < 0.f)
    {
        NormalYaw += 360.f;
    }

    DialValue = FMath::RoundToInt(NormalYaw / StepAngle) % 10;
    float SnappedYaw = DialValue * StepAngle;

    FRotator NewRotation = Rotation;
    NewRotation.Yaw = SnappedYaw;
    SetRelativeRotation(NewRotation);
    // 동기화 할 변수
    DialRotation = SnappedYaw;
    // 호스트 업데이트
    OnRep_Dialvalue();
}

int32 UDialComponent::GetDialValue() const
{
    return DialValue;
}

void UDialComponent::StartInteract(ANumberPlayerController* Controller)
{
    StartMouseX = UNumberUtil::GetMouseX(Controller);
    StartYaw = GetRelativeRotation().Yaw;
}

void UDialComponent::Interact(ANumberPlayerController* Controller)
{
    float CurrentMouseX = UNumberUtil::GetMouseX(Controller);
    float DeltaX = StartMouseX - CurrentMouseX;

    FRotator NewRot = GetRelativeRotation();
    float DeltaYaw = StartYaw + DeltaX;

    Server_SetDialRotation(DeltaYaw);
}

void UDialComponent::EndInteract(ANumberPlayerController* Controller)
{
    FRotator Rotation = GetRelativeRotation();

    Server_SnapDial(Rotation);
}

void UDialComponent::OnRep_Dialvalue()
{
    ACombinationLock* Lock = Cast<ACombinationLock>(GetOwner());
    ANumberHUD* HUD = ANumberHUD::GetHUD(this);
    if (!Lock || !HUD) return;

    HUD->UpdateCurrentPassword(Lock->GetPassword());
}

void UDialComponent::OnRep_DialRotation()
{
    SetRelativeRotation(FRotator(0, DialRotation, 0));
}

void UDialComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(UDialComponent, DialValue);
    DOREPLIFETIME(UDialComponent, DialRotation);

    UNumberUtil::RemoveReplicatedWarning(OutLifetimeProps);
}
