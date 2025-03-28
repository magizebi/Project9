// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberGameModeBase.h"
#include "NumberGameStateBase.h"
#include "NumberPlayerState.h"
#include "NumberPlayerController.h"
#include "NumberHUD.h"
#include "CombinationLock.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "NumberUtil.h"

ANumberGameModeBase::ANumberGameModeBase()
{
	GameStateClass = ANumberGameStateBase::StaticClass();
	PlayerStateClass = ANumberPlayerState::StaticClass();
	PlayerControllerClass = ANumberPlayerController::StaticClass();
	HUDClass = ANumberHUD::StaticClass();

    Password.SetNum(4);
    ServerGameState = nullptr;
}

void ANumberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

    ServerGameState = GetGameState<ANumberGameStateBase>();

    CheckWorldLock();

    StartGame();

    UE_LOG(LogTemp, Warning, TEXT("%d%d%d%d"), Password[0], Password[1], Password[2], Password[3]);
}

bool ANumberGameModeBase::GetVictoryCondition(const TArray<EHitType>& Result) const
{
    for (const EHitType& Hit : Result)
    {
        if (Hit != EHitType::Strike)
        {
            return false;
        }
    }

    return true;
}

void ANumberGameModeBase::NextTurn()
{
    if (!ServerGameState) return;

    if (!ServerGameState->GameData.CurrentTurnPlayer)
    {
        SelectFirstTurnPlayer();
    }
    else
    {
        SelectNextPlayer();
    }

    UpdateLockOwner();
    ServerGameState->GameData.TurnCount++;
}

void ANumberGameModeBase::StartGame()
{
    UNumberUtil::MakeRandomValue(Password);
    
    NextTurn();
}

void ANumberGameModeBase::EndGame()
{
    if (!ServerGameState) return;

    ServerGameState->LockActor->Muticast_UpdateShackle(false);

    // 모든 플레이어 입력 막기
    for (APlayerState* PS : ServerGameState->PlayerArray)
    {
        if (ANumberPlayerState* NumberPlayer = Cast<ANumberPlayerState>(PS))
        {
            if (APlayerController* PC = NumberPlayer->GetPlayerController())
            {
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}

void ANumberGameModeBase::HandleJudgement()
{
    if (!ServerGameState) return;

    ACombinationLock* Lock = ServerGameState->LockActor;

    TArray<int32> GuessPassword = Lock->GetPassword();
    TArray<EHitType> Result = GetPasswordResult(GuessPassword);

    ServerGameState->GameData.PreviousPassword = GuessPassword;
    ServerGameState->GameData.CurrentResult = Result;

    bool bIsVictory = GetVictoryCondition(Result);

    if (bIsVictory)
    {
        EndGame();
    }
    else
    {
        NextTurn();
    }
}

void ANumberGameModeBase::SelectFirstTurnPlayer()
{
    if (!ServerGameState) return;

    int32 Index = FMath::RandRange(0, ServerGameState->PlayerArray.Num() - 1);
    if (!ServerGameState->PlayerArray.IsValidIndex(Index)) return;

    if (ANumberPlayerState* FirstPlayer = Cast<ANumberPlayerState>(ServerGameState->PlayerArray[Index]))
    {
        ServerGameState->GameData.CurrentTurnPlayer = FirstPlayer;
        ServerGameState->OnRep_GameData();
    }

}

void ANumberGameModeBase::SelectNextPlayer()
{
    if (!ServerGameState) return;

    // 현재 플레이어 찾기
    int32 CurrentIndex = ServerGameState->PlayerArray.IndexOfByKey(ServerGameState->GameData.CurrentTurnPlayer);
    int32 NextIndex = CurrentIndex + 1;

    if (NextIndex >= ServerGameState->PlayerArray.Num())
    {
        NextIndex = 0;
    }
    // 다음 플레이어 정하기
    ANumberPlayerState* NextPlayer = Cast<ANumberPlayerState>(ServerGameState->PlayerArray[NextIndex]);
    ServerGameState->GameData.CurrentTurnPlayer = NextPlayer;
}

void ANumberGameModeBase::CheckWorldLock()
{
    TArray<AActor*> LockActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombinationLock::StaticClass(), LockActors);

    ACombinationLock* Lock = nullptr;

    if (LockActors.Num() > 1)
    {
        for (int32 i = 1; i < LockActors.Num(); i++)
        {
            LockActors[i]->Destroy();
        }
        Lock = Cast<ACombinationLock>(LockActors[0]);
    }
    else if (LockActors.Num() == 1)
    {
        Lock = Cast<ACombinationLock>(LockActors[0]);
    }
    else
    {
        Lock = GetWorld()->SpawnActor<ACombinationLock>(ACombinationLock::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    }

    ServerGameState->LockActor = Lock;
}

void ANumberGameModeBase::UpdateLockOwner()
{
    if (!ServerGameState || !ServerGameState->GameData.CurrentTurnPlayer) return;

    APlayerController* CurrentOwner = ServerGameState->GameData.CurrentTurnPlayer->GetPlayerController();
    if (!CurrentOwner) return;

    ServerGameState->LockActor->SetOwner(CurrentOwner);

    for (auto& Player : ServerGameState->PlayerArray)
    {
        ANumberPlayerState* NumberPlayer = Cast<ANumberPlayerState>(Player);
        NumberPlayer->SetMyTurn(false);
    }
    ServerGameState->GameData.CurrentTurnPlayer->SetMyTurn(true);
}

TArray<EHitType> ANumberGameModeBase::GetPasswordResult(const TArray<int32>& Guess)
{
    TArray<EHitType> Result;
    Result.Reserve(4);

    TMap<int32, int32> RestNumber;
    RestNumber.Reserve(4);

    for (int32 Value : Password)
    {
        RestNumber.FindOrAdd(Value)++;
    }

    // 1단계: Strike 처리
    for (int32 Index = 0; Index < Guess.Num(); Index++)
    {
        if (Guess[Index] == Password[Index])
        {
            Result.Add(EHitType::Strike);

            // 카운트 줄이기 (Ball 중복 방지)
            int32 FaultValue = -1;
            int32& Count = UNumberUtil::FindRef<int32, int32>(RestNumber, Guess[Index], FaultValue);
            Count--;
        }
        else
        {
            // 일단 자리 채워두기, Ball 판정은 나중에
            Result.Add(EHitType::None);
        }
    }

    // 2단계: Ball 처리
    for (int32 Index = 0; Index < Guess.Num(); Index++)
    {
        if (Result[Index] != EHitType::None) continue;

        int32 FaultValue = -1;
        int32& Count = UNumberUtil::FindRef<int32, int32>(RestNumber, Guess[Index], FaultValue);
        if (Count > 0)
        {
        Result[Index] = EHitType::Ball;
        Count--;
        }        
    }

    Result.Sort([](EHitType A, EHitType B)
        {
            return A > B;
        });

    return Result;
}
