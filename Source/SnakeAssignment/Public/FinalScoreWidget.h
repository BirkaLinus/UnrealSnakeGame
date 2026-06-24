// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FinalScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEASSIGNMENT_API UFinalScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetFinalScore(int32 Score);
};
