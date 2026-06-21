#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "SnakePawn.generated.h"


class UInputMappingContext;
class UInputAction;
class AGridManager;
class ASnakeGameMode;

UCLASS()
class ASnakePawn : public APawn
{
    GENERATED_BODY()

public:
    ASnakePawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void EnableGame();

private:
    //GRID/POSITION
    UPROPERTY(EditInstanceOnly, Category = "Grid")
    TObjectPtr<AGridManager> GridManager;

    FIntPoint GridPosition;
    FIntPoint Direction;

    //CAMERA Stuff
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;
    
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;
    
    //MOVEMENT Stuff
    FVector TargetLocation;

    float MoveTimer = 0.f;
    
    bool IsOppositeDirection(FIntPoint NewDir) const;
    bool bGameStarted = false;

    UPROPERTY(EditAnywhere)
    float MoveInterval = 0.2f;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* SnakeMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveUpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveDownAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveLeftAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveRightAction;
    
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveRightAction_2;
    
    //VOIDS
    
    void Die();
    
    void MoveSnake();
    void TickMovement(float DeltaTime);

    //Player1
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    
    //Player2
    void MoveRight_2();
    
    void UpdateVisuals(FIntPoint PreviousTail, bool bGrew);
    
    //Food
    UPROPERTY()
    TArray<FIntPoint> SnakeBody;
    
    UPROPERTY(EditAnywhere, Category="Snake")
    TSubclassOf<AActor> SnakeSegmentClass;

    UPROPERTY()
    TArray<AActor*> SnakeSegments;
    
};