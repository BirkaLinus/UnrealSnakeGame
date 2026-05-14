#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "Containers/Array.h"
#include "Math/IntPoint.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

// -------- Grid Cell Struct --------

//OLD VERSION/SYSTEM

// USTRUCT(BlueprintType)
// struct FWallCell
// {
//     GENERATED_BODY()
//     
//     UPROPERTY(EditAnywhere)
//     int32 X = 0;
//     UPROPERTY(EditAnywhere)
//     int32 Y = 0;
// };

//OLD VERSION/SYSTEM

USTRUCT(BlueprintType)
struct FGridCell
{
    GENERATED_BODY()

    // Whether the cell is occupied (snake or food)
    UPROPERTY()
    bool bOccupied = false;
};

USTRUCT(BlueprintType)
struct FLevelData 
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FIntPoint> ObstacleCells;
};
// ---------------------------------

UCLASS(BlueprintType)
class ULevelDataAsset : public UDataAsset
{
    GENERATED_BODY()
    
public: UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FLevelData LevelData;
};

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
    
    //----------------Different grid levels------------------\\
    
    //OLD SYSTEM
    // UPROPERTY(EditAnywhere, Category = "Level")
    // TArray<FWallCell> WallCells;
    //OLD SYSTEM
    
    UPROPERTY(EditAnywhere, Category = "Level")
    ULevelDataAsset* CurrentLevel;
    
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
    
    //Implement different "wall-obstacles" for different levels...
    void SpawnLevelObstacles();

private:
    // Array to store all grid cells
    UPROPERTY()
    TArray<FGridCell> Grid;

    // Converts X,Y to index in Grid array
    int32 GetIndex(int32 X, int32 Y) const;
};