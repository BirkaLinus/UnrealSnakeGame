#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

// -------- Grid Cell Struct --------
USTRUCT(BlueprintType)
struct FGridCell
{
    GENERATED_BODY()

    // Whether the cell is occupied (snake or food)
    UPROPERTY()
    bool bOccupied = false;
};
// ---------------------------------

UCLASS()
class SNAKEASSIGNMENT_API AGridManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGridManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    
    bool IsCellOccupied(int32 X, int32 Y) const;
    void SetCellOccupied(int32 X, int32 Y, bool bOccupied);
    bool IsInsideGrid(int32 X, int32 Y) const;

    // Grid dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GridWidth = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GridHeight = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float CellSize = 100.0f;

    //Blueprint class of tiles/walls to spawn in the grid...
    UPROPERTY(EditAnywhere, Category = "Grid")
    TSubclassOf<AActor> TileClass;
    
    UPROPERTY(EditAnywhere, Category = "Grid")
    TSubclassOf<AActor> WallClass;

        // Converts grid coordinates to world location
    FVector GridToWorld(int32 X, int32 Y) const;
    
    //Food Logics
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FIntPoint FoodPosition;
    
    //Food visuals
    UPROPERTY(EditAnywhere, Category = "Food")
    TSubclassOf<AActor> FoodClass;
    
    UPROPERTY()
    AActor* FoodActor;
    
    void SpawnFood();
    
    bool IsFoodAtPosition(const FIntPoint& Position) const;
    


private:
    // Array to store all grid cells
    UPROPERTY()
    TArray<FGridCell> Grid;

    // Converts X,Y to index in Grid array
    int32 GetIndex(int32 X, int32 Y) const;
};