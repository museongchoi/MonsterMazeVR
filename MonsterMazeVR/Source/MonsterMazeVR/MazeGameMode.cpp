// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGameMode.h"
#include "ExitPortal.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AMazeGameMode::AMazeGameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	TotalSecondsMazeCompletion = 0;
	CurrentLevelIndex = 0;
}

void AMazeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMazeGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	if (LevelName.Contains("TestLevels"))
	{
		StartNextLevel();
		GetWorldTimerManager().SetTimer(MazeCompletionTimerHandle, this, &AMazeGameMode::IncrementMazeCompletionTime, 1.0f, true);
	}
}

void AMazeGameMode::IncrementMazeCompletionTime()
{
	TotalSecondsMazeCompletion++;
}

void AMazeGameMode::LevelComplete()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("TestLevels")));

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Level Complete, seconds since start: %d"), TotalSecondsMazeCompletion));
	//}
	//GetWorldTimerManager().ClearTimer(MazeCompletionTimerHandle);
	//if (CurrentLevel)
	//{
	//	CurrentLevel->Destroy();
	//	CurrentLevelIndex++;
	//	StartNextLevel();
	//}
}

void AMazeGameMode::StartNextLevel()
{
	if (Levels.Num() == 0 || CurrentLevelIndex > Levels.Num() - 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Red, TEXT("END GAME!"));
		return;
	}

	const auto Location = FVector(0, 0, 0);
	const auto Rotation = FRotator(0, 0, 0);
	CurrentLevel = GetWorld()->SpawnActor<AActor>(Levels[CurrentLevelIndex], Location, Rotation);

	for (auto ChildActor : CurrentLevel->GetComponents())
	{
		const auto ChildActorComponent = Cast<UChildActorComponent>(ChildActor);
		if (ChildActorComponent)
		{
			const auto PlayerStart = Cast<APlayerStart>(ChildActorComponent->GetChildActor());
			if (PlayerStart)
			{
				auto MyCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
				MyCharacter->SetActorTransform(PlayerStart->GetActorTransform());
			}

			auto ExitPortal = Cast<AExitPortal>(ChildActorComponent->GetChildActor());
			if (ExitPortal)
			{
				ExitPortal->LevelComplete.AddDynamic(this, &AMazeGameMode::LevelComplete);
			}
		}
	}
}


