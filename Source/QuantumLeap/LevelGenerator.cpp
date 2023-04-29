// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "PlatformActor.h"
#include "WallActor.h"
#include "osu.hpp"


// Sets default values for this component's properties
ULevelGenerator::ULevelGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevelGenerator::BeginPlay()
{
	Super::BeginPlay();


	//fetch osu data

	//using namespace osu;
	//Beatmap beats("/Game/OsuMaps/Pharmacist.osu");
	



	//for beat and slider in osu map do this:
	FVector location = FVector(0.0f, 0.0f, 0.0f);
	FVector zerovec(0, 0, 0);
	//for (const auto& hitobj : beats.HitObjects()) {
	for (int i = 0; i < 500; i++) {
		//if (hitobj.Type() == HitObjectType::CIRCLE) {
		if(i%2 == 0){
			for (int j = 0; j < 5; j++) {
				APlatformActor* platform = (APlatformActor*)GetWorld()->SpawnActor(APlatformActor::StaticClass(), &zerovec);
				FVector scale(1.0f, 1.0f, 1.0f);
				platform->SetActorScale3D(scale);
				float xval = FMath::FRandRange(-200, 200);

				FVector origin;
				FVector extent;
				platform->GetActorBounds(false, origin, extent);
				location.Y += extent.Y / 2;
				platform->SetActorLocation(location);
				location += FVector(xval, PlatformYSpacing + extent.Y / 2, PlatformZSpacing);
			}
		}
		else {
			for (int j = 0; j < 5; j++) {
				AWallActor* wall = (AWallActor*)GetWorld()->SpawnActor(AWallActor::StaticClass(), &zerovec);
				FVector scale(1.0f, 5.0f, 1.0f);
				wall->SetActorScale3D(scale);

				FVector origin;
				FVector extent;
				wall->GetActorBounds(false, origin, extent);
				location.Y += extent.Y / 2;

				FVector offset(location.X + WallSpacing, location.Y, location.Z);

				wall->SetActorLocation(offset);
				location += FVector(0, PlatformYSpacing * 1.5 + extent.Y / 2, 0);
				WallSpacing *= -1;
			}

		}
		
	}
	
	// ...
	
}


// Called every frame
void ULevelGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

