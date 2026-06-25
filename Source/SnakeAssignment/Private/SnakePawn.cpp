#include "SnakePawn.h"

#include "AssetDefinitionAssetInfo.h"
#include "GridManager.h"
#include "SnakeGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "Components/BoxComponent.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"

#include "GameFramework/PlayerController.h"

// Sets default values
ASnakePawn::ASnakePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    // AutoPossessPlayer = EAutoReceiveInput::Player0;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    
    HeadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollision"));
    HeadCollision->SetupAttachment(RootComponent);
    HeadCollision->SetBoxExtent(FVector(10, 10, 10));
    HeadCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    HeadCollision->SetGenerateOverlapEvents(true);
    
    //Camera
    //Create SpringArm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    
    SpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
    SpringArm->TargetArmLength = 800.f;  
    SpringArm->bDoCollisionTest = false;
    
    //Camera Rotation locked
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritRoll = false;
    SpringArm->bInheritYaw = false;
    SpringArm->SetUsingAbsoluteRotation(true);
    
    //Create Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ASnakePawn::BeginPlay()
{
    Super::BeginPlay();
    
    GridManager = Cast<AGridManager>(
    UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

    UE_LOG(LogTemp, Warning, TEXT("SnakeGameMode Active"));
    
    if (!GridManager)
    {
        UE_LOG(LogTemp, Error, TEXT("GridManager not assigned"));
        return;
    }

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    
    if (!PlayerController) return;
    
    ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
    if (LocalPlayer)
    {
        int32 PlayerIndex = GetGameInstance()->GetLocalPlayers().IndexOfByKey(LocalPlayer);
        
        TArray<ULocalPlayer*> Players = GetGameInstance()->GetLocalPlayers();
        
        UE_LOG(LogTemp, Warning, TEXT("PlayerIndex: %d"), PlayerIndex);
        
        if (PlayerIndex == 0)
        {
            //Snake Starts at center...
            GridPosition = FIntPoint(
        GridManager->GridWidth / 4,
        GridManager->GridHeight / 4
            );
        
            UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        
            if (Subsystem)
            {
                Subsystem->AddMappingContext(SnakeMappingContext, 0);
                UE_LOG(LogTemp, Warning, TEXT("PlayerIndex: was 0"));
            }
        }
        else
        {
            GridPosition = FIntPoint(
        GridManager->GridWidth / 2,
        GridManager->GridHeight / 2
            );
            UE_LOG(LogTemp, Warning, TEXT("Player2 was enabled!!!"));
            EnableGame();
        }
    }

    SnakeBody.Add(GridPosition);

    //Marks starting cell as occupied
    GridManager->SetCellOccupied(GridPosition.X, GridPosition.Y, true);

    Direction = FIntPoint(1, 0);

    SetActorLocation(
        GridManager->GridToWorld(GridPosition.X, GridPosition.Y)
    );
    
}

// Called every frame
void ASnakePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bGameStarted) return; //No movement unless game started.
    
    TickMovement(DeltaTime);
}

// Input binding
void ASnakePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    
    if (!PlayerController) return;
    
    ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
    
    if (LocalPlayer)
    {
        int32 PlayerIndex = GetGameInstance()->GetLocalPlayers().IndexOfByKey(LocalPlayer);
        
        if (PlayerIndex == 1)
        {
            return;
        }
    }
    
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    
    if (Input)
    {
        Input->BindAction(MoveUpAction, ETriggerEvent::Started, this, &ASnakePawn::MoveUp);
        Input->BindAction(MoveDownAction, ETriggerEvent::Started, this, &ASnakePawn::MoveDown);
        Input->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ASnakePawn::MoveLeft);
        Input->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ASnakePawn::MoveRight);
        
        //2
        Input->BindAction(MoveRightAction_2, ETriggerEvent::Started, this, &ASnakePawn::MoveRight_2);
    }
}

void ASnakePawn::EnableGame()
{
    bGameStarted = true; //Enables the movement from snakegamemode... (when starting the game)
}

bool ASnakePawn::IsOppositeDirection(FIntPoint NewDir) const
{
    return (NewDir.X == -Direction.X && NewDir.Y == -Direction.Y);
}

// Input functions

//Special solution, logic is not correct, but it's the most simple fix for the movement to work as intended (Maybe fix
//later when/if time is there.)
void ASnakePawn::MoveUp()
{
    if (!bGameStarted) return; //No movement unless game started.
    
    //Direction = FIntPoint(0, 1);
    //Direction = FIntPoint(1, 0); //SpecialSolution
    
    //Making sure I cant turn back into the snake
    FIntPoint NewDir = FIntPoint (1,0);
    
    if (!IsOppositeDirection(NewDir))
    {
        Direction = NewDir;
    }
    
}

void ASnakePawn::MoveDown()
{
    if (!bGameStarted) return; //No movement unless game started.
    
    //Direction = FIntPoint(0, -1);
    //Direction = FIntPoint(-1, 0); //SpecialSolution
    
    //Making sure I cant turn back into the snake
    FIntPoint NewDir = FIntPoint (-1, 0);
    
    if (!IsOppositeDirection(NewDir))
    {
        Direction = NewDir;
    }
}

void ASnakePawn::MoveLeft()
{
    if (!bGameStarted) return; //No movement unless game started.
    
    //Direction = FIntPoint(-1, 0);
    //Direction = FIntPoint(0, -1); //SpecialSolution
    
    //Making sure I cant turn back into the snake
    FIntPoint NewDir = FIntPoint (0, -1);
    
    if (!IsOppositeDirection(NewDir))
    {
        Direction = NewDir;
    }
}

void ASnakePawn::MoveRight()
 {
     if (!bGameStarted) return; //No movement unless game started.
     
     //Direction = FIntPoint(1, 0);
     //Direction = FIntPoint(0, 1); //SpecialSolution
     
     //Making sure I cant turn back into the snake
     FIntPoint NewDir = FIntPoint(0, 1);
     
     if (!IsOppositeDirection(NewDir))
     {
         Direction = NewDir;
     }
 }

void ASnakePawn::MoveRight_2()
{
    if (!bGameStarted) return; //No movement unless game started.
    
//     APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 1);
//
//     if (PC)
//     {
//         APawn* Pawn = PC->GetPawn();
//
//         if (Pawn)
//         {
//             ASnakePawn* SnakePawn = Cast<ASnakePawn>(Pawn);
//             
//             SnakePawn->MoveRight();
//         }
//     }
//     //Direction = FIntPoint(1, 0);
//     //Direction = FIntPoint(0, 1); //SpecialSolution
//     
//     //Making sure I cant turn back into the snake
//     // FIntPoint NewDir = FIntPoint(0, 1);
//     
//     // if (!IsOppositeDirection(NewDir))
//     // {
//     //     Direction = NewDir;
//     // }
}

// Movement

void ASnakePawn::MoveSnake()
{    
    
    if (!GridManager)
{
    UE_LOG(LogTemp, Error, TEXT("MoveSnake(): GridManager is NULL"));
    return;
}
    FIntPoint PreviousTail = SnakeBody.Last();
    
    FIntPoint NewHead = GridPosition + Direction;
    
    //CHECK IF ENTERING A WALL
    if (!GridManager->IsInsideGrid(NewHead.X, NewHead.Y))
    {
        Die();
        return;
    }
    
    //FOOD
    bool bAteFood = bPendingFoodEaten; //Eats the food and put it to false straight away and unr the logics for eating straight after...
    bPendingFoodEaten = false;
    
    
    //GAMLA SÄTTET
    // bool bAteFood = GridManager->IsFoodAtPosition(NewHead);
    
    //SELFCOLLIDE
    bool bIsOccupied = GridManager->IsCellOccupied(NewHead.X, NewHead.Y);
    
    if (bIsOccupied && !( !bAteFood && NewHead == PreviousTail))
    {
        Die();
        return;
    }
    
    //MOVE
    SnakeBody.Insert(NewHead, 0);
    GridPosition = NewHead;
    
    GridManager->SetCellOccupied(NewHead.X, NewHead.Y, true);
    
    if (!bAteFood)
    {
        SnakeBody.Pop();
        
        GridManager->SetCellOccupied(PreviousTail.X, PreviousTail.Y, false);
    }
    if (bAteFood)
    {
        UE_LOG(LogTemp, Warning, TEXT("Food eaten"));
    
        GridManager->SpawnFood();
    
        if (ASnakeGameMode* GM =
            Cast<ASnakeGameMode>(UGameplayStatics::GetGameMode(this)))
        {
            UE_LOG(LogTemp, Warning, TEXT("Found GameMode"));
    
            GM->IncreaseScore();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GameMode not found"));
        }
    }
    
    TargetLocation = GridManager->GridToWorld(NewHead.X, NewHead.Y);
    
    UpdateVisuals(PreviousTail, bAteFood);
}

void ASnakePawn::TickMovement(float DeltaTime)
{
    
    if (!GridManager)
    {
        UE_LOG(LogTemp, Error, TEXT("TickMovement: GridManager is NULL"));
        return;
    }
    
    MoveTimer += DeltaTime;

    if (MoveTimer >= MoveInterval)
    {
        MoveTimer = 0.f;
        MoveSnake(); // updates SnakeBody (logical grid state)
    }

    //Snake head on grid
    FVector HeadTarget = GridManager->GridToWorld(
        GridPosition.X,
        GridPosition.Y
    );

    FVector CurrentHead = GetActorLocation();

    //How quickly the snake visual moves - ish...
    FVector NewHeadLocation = FMath::VInterpTo(
        CurrentHead,
        HeadTarget,
        DeltaTime,
        10.0f
    );

    SetActorLocation(NewHeadLocation);


    //Move the "tails" of the snake
    for (int32 i = 0; i < SnakeSegments.Num(); i++)
    {
        if (!SnakeBody.IsValidIndex(i + 1)) continue;

        FVector Target = GridManager->GridToWorld(
            SnakeBody[i + 1].X,
            SnakeBody[i + 1].Y
        );

        FVector Current = SnakeSegments[i]->GetActorLocation();

        FVector NewLocation = FMath::VInterpTo(
            Current,
            Target,
            DeltaTime,
            10.0f
        );

        SnakeSegments[i]->SetActorLocation(NewLocation);
    }
}

void ASnakePawn::UpdateVisuals(FIntPoint PreviousTail, bool bGrew)
{
    if (!GridManager)
    {
        UE_LOG(LogTemp, Error, TEXT("UpdateVisuals(): GridManager is NULL"));
        return;
    }
    
    while (SnakeSegments.Num() < SnakeBody.Num() - 1)
    {
        AActor* NewSeg = GetWorld()->SpawnActor<AActor>(SnakeSegmentClass);

        if (bGrew)
        {
            FVector SpawnPos = GridManager->GridToWorld(PreviousTail.X, PreviousTail.Y);

            NewSeg->SetActorLocation(SpawnPos);
        }

        SnakeSegments.Add(NewSeg);
    }
}

void ASnakePawn::Die()
{
    UE_LOG(LogTemp, Warning, TEXT("Snake Dead"));
    
    //Get The GameMode
    ASnakeGameMode* GM = Cast<ASnakeGameMode>(UGameplayStatics::GetGameMode(this));
    if (GM)
    {
        GM->GameOver(); //Calls the GameOver in GM.
    }
    SetActorTickEnabled(false);
}

void ASnakePawn::NotifyFoodEaten(AActor* EatenFood)
{
    bPendingFoodEaten = true;
}
