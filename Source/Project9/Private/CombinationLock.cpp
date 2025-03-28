// Fill out your copyright notice in the Description page of Project Settings.


#include "CombinationLock.h"
#include "DialComponent.h"
#include "ButtonComponent.h"
#include "LEDLightComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// 생성자: 기본 설정
ACombinationLock::ACombinationLock()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    // LockBody (자물쇠 본체)
    LockBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockBody"));
    SetRootComponent(LockBody);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(LockBody);
    SpringArm->bUsePawnControlRotation = false;
    // 🔹 카메라 추가
    LockCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LockCamera"));
    LockCamera->SetupAttachment(SpringArm); // LockBody에 부착
    LockCamera->bUsePawnControlRotation = false;

    Shackle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shackle"));
    Shackle->SetIsReplicated(true);
    Shackle->SetupAttachment(LockBody);

    Button = CreateDefaultSubobject<UButtonComponent>(TEXT("Button"));
    Button->SetupAttachment(LockBody);

    Dials.SetNum(4);
    Lights.SetNum(4);

    for (auto& Dial : Dials)
    {
        FName DialName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), TEXT("Dial"));
        Dial = CreateDefaultSubobject<UDialComponent>(DialName);
        Dial->SetupAttachment(LockBody);
    }

    for (auto& Light : Lights)
    {
        FName LightName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), TEXT("LEDLight"));
        Light = CreateDefaultSubobject<ULEDLightComponent>(LightName);
        Light->SetupAttachment(LockBody);
    }

    ShackleLocation = Shackle->GetRelativeLocation().Z;
    ShackleHeight = 100.f;
}

void ACombinationLock::BeginPlay()
{
    Super::BeginPlay();

    for (auto& Light : Lights)
    {
        Light->SetColor(FLinearColor::White);
        Light->SetBrightness(2.f);
        Light->SetSwitch(false);
    }

    /*if (HasAuthority())
    {
        Server_UpdateShackle(bIsLocked);
    }*/
}

void ACombinationLock::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    AttachComponentsToSockets();
}

void ACombinationLock::Tick(float DeltaTime)
{
}

UCameraComponent* ACombinationLock::GetCamera() const
{
    return LockCamera;
}

UDialComponent* ACombinationLock::GetDial(int32 Index) const
{
    return Dials[Index];
}

ULEDLightComponent* ACombinationLock::GetLight(int32 Index) const
{
    return Lights[Index];
}

int32 ACombinationLock::GetDialAmount() const
{
    return Dials.Num();
}

TArray<int32> ACombinationLock::GetPassword() const
{
    TArray<int32> Password;
    Password.Reserve(4);

    for (auto& Dial : Dials)
    {
        Password.Add(Dial->GetDialValue());
    }

    return Password;
}

void ACombinationLock::AttachComponentsToSocket(USceneComponent* Component, FName SocketName)
{
    if (!Component) return;

    if (LockBody->DoesSocketExist(SocketName))
    {
        Component->AttachToComponent(LockBody, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
    }
}

// 소켓을 통해 다이얼과 섀클을 LockBody에 부착
void ACombinationLock::AttachComponentsToSockets()
{
    if (!LockBody) return;
    
    for (int i = 0; i < 4; i++)
    {
        FName SocketName = FName(*FString::Printf(TEXT("Dial%d"), i + 1));
        AttachComponentsToSocket(Dials[i], SocketName);

        SocketName = FName(*FString::Printf(TEXT("Light%d"), i + 1));
        AttachComponentsToSocket(Lights[i], SocketName);
    }
    AttachComponentsToSocket(Shackle, "Shackle");
}

void ACombinationLock::Muticast_UpdateShackle_Implementation(bool Value)
{
    FVector Location = Shackle->GetRelativeLocation();
    Location.Z = Value ? ShackleLocation - ShackleHeight : ShackleLocation;
    Shackle->SetRelativeLocation(Location);
}
