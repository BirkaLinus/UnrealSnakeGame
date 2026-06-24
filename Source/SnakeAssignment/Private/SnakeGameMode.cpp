// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"

#include "ScoreWidget.h"
#include "SnakePawn.h"
#include "GridManager.h"
#include "Kismet/GameplayStatics.h"

void ASnakeGameMode::SetGameState(EGameState NewState)
{
	CurrentGameState = NewState;
	
	switch (CurrentGameState)
	{
	case EGameState::MainMenu:
		UE_LOG(LogTemp, Warning, TEXT("GameState: MainMenu"));
		break;
		
	case EGameState::Playing:
		UE_LOG(LogTemp, Warning, TEXT("GameState: Playing"));
		break;
		
	case EGameState::GameOver:
		UE_LOG(LogTemp, Warning, TEXT("GameState: GameOver"));
		break;
	}
}

void ASnakeGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetGameState(EGameState::MainMenu);

	GridManagerRef = Cast<AGridManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass())
	);
	
	//Spawn snake2 and destroy it if not multiplayer, random fix IDK..
	Player2Controller = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);

	if (MainMenuWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
	}
}

void ASnakeGameMode::ToggleTwoPlayers()
{
	bTwoPlayers = !bTwoPlayers;
	
	UE_LOG(LogTemp, Warning, TEXT("TwoPlaters : %s"), bTwoPlayers ? TEXT("TRUE") : TEXT("FALSE"));
}

void ASnakeGameMode::StartGame()
{
	SetGameState(EGameState::Playing);

	UE_LOG(LogTemp, Warning, TEXT("StartGame called. bTwoPlayers = %s"),
		bTwoPlayers ? TEXT("TRUE") : TEXT("FALSE"));

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	if (ScoreWidgetClass)
	{
		ScoreWidget = CreateWidget<UScoreWidget>(
			GetWorld()->GetFirstPlayerController(),
			ScoreWidgetClass
		);

		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WIDGET FAILED TO CREATE"));
		}
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
	
	Snake2 = Player2Controller ? Cast<ASnakePawn>(Player2Controller->GetPawn()) : nullptr;

	//Place to destroy the secondsnake if its not multiplayer.
	DestroyUnneededSecondSnake();

	ASnakePawn* Pawn = Cast<ASnakePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Pawn)
	{
		Pawn->EnableGame();
	}

	if (bTwoPlayers && Snake2)
	{
		Snake2->EnableGame();
	}
}

void ASnakeGameMode::DestroyUnneededSecondSnake()
{
	if (bTwoPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyUnneededSecondSnake: bTwoPlayers TRUE, skipping"));
		return;
	}

	ASnakePawn* P2Pawn = Player2Controller
		? Cast<ASnakePawn>(Player2Controller->GetPawn())
		: nullptr;

	if (P2Pawn)
	{
		//Remove the occupied cells of snake2 (fixed bug with "invisible snake" in the center.
		if (GridManagerRef)
		{
			for (const FIntPoint& Cell : P2Pawn->GetSnakeBody())
			{
				GridManagerRef->SetCellOccupied(Cell.X, Cell.Y, false);
			}
		}

		for (AActor* Segment : P2Pawn->GetSnakeSegments())
		{
			if (Segment)
			{
				Segment->Destroy();
			}
		}
		P2Pawn->Destroy();
	}

	Snake2 = nullptr;
}

void ASnakeGameMode::GameOver()
{
	SetGameState(EGameState::GameOver);

	//Restarting with some delay...
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASnakeGameMode::RestartGame,
		1.5f, //The timer
		false
	);
}

void ASnakeGameMode::RestartGame()
{
	UWorld* World = GetWorld();
	
	if (World)
	{
		FString CurrentLevel = World->GetName();
		
		UGameplayStatics::OpenLevel(this, FName(*CurrentLevel));
	}
}

void ASnakeGameMode::Level1()
{
	UE_LOG(LogTemp, Warning, TEXT("Level1() called"));

	if (GridManagerRef && Level1Data)
	{
		GridManagerRef->LoadLevel(Level1Data);
		StartGame();
	}
}

void ASnakeGameMode::Level2()
{
	if (GridManagerRef && Level2Data)
	{
		GridManagerRef->LoadLevel(Level2Data);
		StartGame();
	}
}

void ASnakeGameMode::Level3()
{
	if (GridManagerRef && Level3Data)
	{
		GridManagerRef->LoadLevel(Level3Data);
		StartGame();
	}
}

void ASnakeGameMode::IncreaseScore()
{
	Score++;

	UE_LOG(LogTemp, Warning, TEXT("Increased Score: %d"), Score);

	if (ScoreWidget)
	{
		ScoreWidget->SetScore(Score);
	}
}

void ASnakeGameMode::FinalScore()
{
	
}



