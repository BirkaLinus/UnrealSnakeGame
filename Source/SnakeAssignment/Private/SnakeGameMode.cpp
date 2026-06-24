// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"

#include "FinalScoreWidget.h"
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

void ASnakeGameMode::EnterMainMenu()
{
	//Resets the game
	RestartGame();
	SetGameState(EGameState::MainMenu);
	
	if (FinalScoreWidget)
	{
		FinalScoreWidget->RemoveFromParent();
	}
	
	if (MainMenuWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ASnakeGameMode::ToggleTwoPlayers()
{
	bTwoPlayers = !bTwoPlayers;
	
	UE_LOG(LogTemp, Warning, TEXT("TwoPlaters : %s"), bTwoPlayers ? TEXT("TRUE") : TEXT("FALSE"));
}

void ASnakeGameMode::StartGame() //REMEMBER TO FIX VOID QUITGAME AND LINK IT IN THE BP WIDGET
{
	SetGameState(EGameState::Playing);
	
	UE_LOG(LogTemp, Warning,
	TEXT("StartGame called. bTwoPlayers = %s"),
	bTwoPlayers ? TEXT("TRUE") : TEXT("FALSE"));
	
	//Check how many players
	
	//---------DOESNT WORK FOR NOW-----------
	
	// if (bTwoPlayers)
	// {
	// 	UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	// }
	
	//---------DOESNT WORK FOR NOW-----------
	
	UE_LOG(LogTemp, Warning,
	TEXT("Two Players = %s"),
	bTwoPlayers ? TEXT("TRUE") : TEXT("FALSE"));
	
	//Remove the MainMenu UI
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentWidget is NULL in StartGame!"));
	}
	
	if (ScoreWidgetClass)
	{
		ScoreWidget = CreateWidget<UScoreWidget>(
			GetWorld()->GetFirstPlayerController(),
			ScoreWidgetClass
		);
		
		UE_LOG(LogTemp, Warning, TEXT("ScoreWidget created: %s"),
	ScoreWidget ? TEXT("YES") : TEXT("NO"));

		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("WIDGET ADDED"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WIDGET FAILED TO CREATE"));
		}
		
		if (!ScoreWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("ScoreWidgetClass is NULL"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ScoreWidgetClass is VALID"));
		}
	}
	
	//Gameplay Logic instead
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
	
	 ASnakePawn* Pawn = Cast<ASnakePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	 if (bTwoPlayers && SnakePawnClass)
	 {
	 	UE_LOG(LogTemp, Warning, TEXT("Trying to spawn Snake2"));
	
	 	 APlayerController* SecondPC=UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	 	
	 	Snake2 = GetWorld()->SpawnActor<ASnakePawn>(
	 		SnakePawnClass,
	 		FVector(2500.f, 0.f, 0.f),
	 		FRotator::ZeroRotator
	 	);
	
	 	UE_LOG(LogTemp, Warning,
	 		TEXT("Snake2 pointer = %s"),
	 		Snake2 ? TEXT("VALID") : TEXT("NULL"));
	
	 	if (Snake2)
	 	{
	 		SecondPC->Possess(Snake2);
	 		UE_LOG(LogTemp, Warning, TEXT("Spawned Snake2"));
	 		Snake2->EnableGame();
	 	}
	}
	
	if (Pawn)
	{
		Pawn->EnableGame();
	}
}

void ASnakeGameMode::GameOver()
{
	SetGameState(EGameState::GameOver);
	
	
	
	//Restarting with some delay...
	FTimerHandle TimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(
	TimerHandle,
	this,
	&ASnakeGameMode::FinalScore,
	1.5f, //The timer
	false
);
	
	// GetWorld()->GetTimerManager().SetTimer(
	// 	TimerHandle,
	// 	this,
	// 	&ASnakeGameMode::RestartGame,
	// 	1.5f, //The timer
	// 	false
	// );
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
	UE_LOG(LogTemp, Warning, TEXT("FinalScore() Called"));
	
	if (ScoreWidget)
	{
		ScoreWidget->RemoveFromParent();
	}
	
	if (FinalScoreWidgetClass)
	{
		FinalScoreWidget = CreateWidget<UFinalScoreWidget>(GetWorld()->GetFirstPlayerController(), FinalScoreWidgetClass);
		
		UE_LOG(LogTemp, Warning, TEXT("FinalScoreWidget created: %s"), FinalScoreWidget ? TEXT("YES") : TEXT("NO"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FinalScoreWidgetClass is NULL"));
	}
	
	if (FinalScoreWidget)
	{
		FinalScoreWidget->AddToViewport();
		FinalScoreWidget->SetFinalScore(Score);
		UE_LOG(LogTemp, Warning, TEXT("FinalScoreWidget added to viewport"));
	}
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeGameOnly());
	}
	
}



