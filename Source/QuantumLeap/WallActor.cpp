// Fill out your copyright notice in the Description page of Project Settings.


#include "WallActor.h"

// Sets default values
AWallActor::AWallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(VisualMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroidVisualAsset(TEXT("/Game/Models/GlowWall"));

	if (AsteroidVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(AsteroidVisualAsset.Object);
		//VisualMesh->SetWorldScale3D(FVector(100, 100, 100));
		//VisualMesh->SetRelativeScale3D(FVector(100, 100, 100));
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

