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

UCLASS()
class SNAKEASSIGNMENT_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category ="UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	virtual void BeginPlay() override;
	
	void SetGameState(EGameState NewState);
	
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
