// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"

#include "SnakePawn.h"
#include "Components/BoxComponent.h"

// Sets default values
AFood::AFood()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetBoxExtent(FVector(1,1,1)); //Finally understood that I had to manually change it to make it actually smaller... :D
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFood::OnOverLap);
	CollisionComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::OnOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASnakePawn* Snake = Cast<ASnakePawn>(OtherActor))
	{
		Snake->NotifyFoodEaten(this);
	}
}

