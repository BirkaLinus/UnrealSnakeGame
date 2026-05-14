#include "GridManager.h"
#include "Engine/World.h"

// Sets default values
AGridManager::AGridManager()
{
    // Turn off Tick (not needed for static grid)
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize the grid array
    Grid.SetNum(GridWidth * GridHeight);
    for (int32 i = 0; i < Grid.Num(); i++)
    {
        Grid[i].bOccupied = false;
    }
    
    //Initialize the border walls
    for (int32 X = 0; X < GridWidth; X++)
    {
        SetCellOccupied(X, 0, true);
        SetCellOccupied(X, GridHeight - 1, true);

        FVector Top = GridToWorld(X, 0);
        FVector Bottom = GridToWorld(X, GridHeight - 1);

        GetWorld()->SpawnActor<AActor>(WallClass, Top, FRotator::ZeroRotator);
        GetWorld()->SpawnActor<AActor>(WallClass, Bottom, FRotator::ZeroRotator);
    }

    for (int32 Y = 0; Y < GridHeight; Y++)
    {
        SetCellOccupied(0, Y, true);
        SetCellOccupied(GridWidth - 1, Y, true);

        FVector Left = GridToWorld(0, Y);
        FVector Right = GridToWorld(GridWidth - 1, Y);

        GetWorld()->SpawnActor<AActor>(WallClass, Left, FRotator::ZeroRotator);
        GetWorld()->SpawnActor<AActor>(WallClass, Right, FRotator::ZeroRotator);
    }
    
    //Custom Level walls/Obstacles
    SpawnLevelObstacles();

    // Safety check
    if (!TileClass) return;

    // Spawn tiles
    for (int32 Y = 0; Y < GridHeight; Y++)
    {
        for (int32 X = 0; X < GridWidth; X++)
        {
            FVector Location = GridToWorld(X, Y);
            GetWorld()->SpawnActor<AActor>(
                TileClass,
                Location,
                FRotator::ZeroRotator
            );
        }
    }
    
    
    SpawnFood(); //Makes sure to spawn food
}

// Converts grid X,Y to index in the 1D Grid array
int32 AGridManager::GetIndex(int32 X, int32 Y) const
{
    return Y * GridWidth + X;
}

// Converts grid coordinates to world location (centers grid at origin)
FVector AGridManager::GridToWorld(int32 X, int32 Y) const
{
    FVector Offset = FVector(
        -GridWidth * CellSize * 0.5f + CellSize * 0.5f,
        -GridHeight * CellSize * 0.5f + CellSize * 0.5f,
        0.f
    );

    return Offset + FVector(
        X * CellSize,
        Y * CellSize,
        0.f
    );
}

void AGridManager::SpawnFood()
{
    TArray<FIntPoint> FreeCells;
    
    for (int32 Y=0; Y < GridHeight; Y++)
    {
        for (int32 X = 0; X < GridWidth; X++)
        {
            int32 Index = GetIndex(X, Y);
            if (!IsCellOccupied(X, Y))
            {
                FreeCells.Add(FIntPoint(X, Y));
            }
        }
    }
    
    //Snake covers map
    if (FreeCells.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No free cells for food!"));
        return;
    }
    
    //Choose the random cell for food
    int32 RandIndex = FMath::RandRange(0, FreeCells.Num() - 1);
    FoodPosition = FreeCells[RandIndex];
    
    FVector WorldPos = GridToWorld(FoodPosition.X, FoodPosition.Y);
    
    if (!FoodClass || !GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("FoodClass not assigned or world invalid"));
        return;
    }
    
    if (FoodActor)
    {
        FoodActor->SetActorLocation(WorldPos);
    }
    else
    {
        FoodActor = GetWorld()->SpawnActor<AActor>(FoodClass, WorldPos, FRotator::ZeroRotator);
    }
}

bool AGridManager::IsFoodAtPosition(const FIntPoint& Position) const
{
    return Position == FoodPosition;
}

bool AGridManager::IsCellOccupied(int32 X, int32 Y) const
{
    if (!IsInsideGrid(X, Y)) return false;
    
    return Grid[GetIndex(X, Y)].bOccupied;
}

void AGridManager::SetCellOccupied(int32 X, int32 Y, bool bOccupied)
{
    if (!IsInsideGrid(X, Y)) return;
    
    Grid[GetIndex(X, Y)].bOccupied = bOccupied;
}

bool AGridManager::IsInsideGrid(int32 X, int32 Y) const
{
    return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

void AGridManager::SpawnLevelObstacles()
{
    if (!CurrentLevel || !WallClass || !GetWorld()) return;
    
    const TArray<FIntPoint>& Obstacles = CurrentLevel->LevelData.ObstacleCells;
    
    for (const FIntPoint& Cell : Obstacles)
    {
        //Marks the location as occupied
        SetCellOccupied(Cell.X, Cell.Y, true);
        
        //Spawn the Walls/Obstacles visually
        FVector Location = GridToWorld(Cell.X, Cell.Y);
        GetWorld()->SpawnActor<AActor>(WallClass, Location, FRotator::ZeroRotator);
    }
}