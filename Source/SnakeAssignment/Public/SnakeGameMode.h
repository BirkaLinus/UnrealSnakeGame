// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Blueprint/UserWidget.h"

#include "SnakeGameMode.generated.h"



UENUM(BlueprintType)
enum class EGameState : uint8
{
	MainMenu,
	Playing,
	GameOver
};

class ULevelDataAsset;
class AGridManager;
class ASnakePawn;

UCLASS()
class SNAKEASSIGNMENT_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	AGridManager* GridManagerRef; //Reference to the gridmanager...
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category ="UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	virtual void BeginPlay() override;
	
	void SetGameState(EGameState NewState);
	
	UPROPERTY(BlueprintReadWrite)
	bool bTwoPlayers = false;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASnakePawn> SnakePawnClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Levels")
	ULevelDataAsset* Level1Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Levels")
	ULevelDataAsset* Level2Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Levels")
	ULevelDataAsset* Level3Data;
	
	UFUNCTION(BlueprintCallable)
	void SetTwoPlayers(bool bEnabled)
	{
		bTwoPlayers = bEnabled;
	}
	
	UFUNCTION(BlueprintCallable)
	void ToggleTwoPlayers();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void Level1();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void Level2();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void Level3();
	
	UFUNCTION(BlueprintCallable, Category ="Game")
	void StartGame();
	
	UFUNCTION(BlueprintCallable, Category ="Game")
	void GameOver();
	
	UFUNCTION(BlueprintCallable, Category ="Game")
	void RestartGame();
	
private:
	EGameState CurrentGameState;
	UUserWidget* CurrentWidget;
};
