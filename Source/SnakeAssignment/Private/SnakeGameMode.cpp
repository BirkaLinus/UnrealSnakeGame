// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"

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
	if (bTwoPlayers)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	}
	
	UE_LOG(LogTemp, Warning,
	TEXT("Two Players = %s"),
	bTwoPlayers ? TEXT("TRUE") : TEXT("FALSE"));
	
	//Remove the MainMenu UI
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}
	
	//Gameplay Logic instead
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
	
	ASnakePawn* Pawn = Cast<ASnakePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	
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



