// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformActor.h"

// Sets default values
APlatformActor::APlatformActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(VisualMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroidVisualAsset(TEXT("/Game/Models/PrototypePlatform"));

	if (AsteroidVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(AsteroidVisualAsset.Object);
		//VisualMesh->SetWorldScale3D(FVector(100, 100, 100));
		//VisualMesh->SetRelativeScale3D(FVector(100, 100, 100));
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void APlatformActor::BeginPlay()
{
	Super::BeginPlay();



	//FVector location(0, 0, 0);
	//APlatformActor* platform = (APlatformActor*)GetWorld()->SpawnActor(APlatformActor::StaticClass(), &location);
	//FVector scale(1000, 1000, 1000);
	//platform->SetActorScale3D(scale);
	
}

// Called every frame
void APlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

