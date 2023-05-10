// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "osu.hpp"
#include "PlatformActor.h"
#include "WallActor.h"
#include "spinner.h"
#include "memphis.hpp"
#include "ToU.hpp"
#include "Kismet/GameplayStatics.h"/*
#include "Sound/SoundCue.h"
#include "Sound/Soundbase.h"
#include "Components/AudioComponent.h"*/






// Sets default values for this component's properties
ULevelGenerator::ULevelGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor* ULevelGenerator::ConstructPlatform(float xpos, int time){
	//UE_LOG(LogTemp, Warning, TEXT("The float value is: %d"), time);
	FVector zerovec(0, 0, 0);

	//spawning platform in
	APlatformActor* platform = (APlatformActor*)GetWorld()->SpawnActor(APlatformActor::StaticClass(), &zerovec);

	//setting scale
	FVector scale(1.0f, 1.0f, 1.0f);
	platform->SetActorScale3D(scale);

	//setting xval (how far left and right to jump)
	/*float xval = FMath::FRandRange(-200, 200);*/

	//get bounds
	FVector origin;
	FVector extent;
	platform->GetActorBounds(false, origin, extent);


	
	//set location in world
	FVector location(xpos, time, 0);
	platform->SetActorLocation(location);
	
	//location += FVector(xval, PlatformYSpacing + extent.Y / 2, PlatformZSpacing);
	return platform;
}



AActor* ULevelGenerator::ConstructWall(float xpos, int time, int length, int type) {
	using namespace osu;
	//UE_LOG(LogTemp, Warning, TEXT("wall length: %d"), length);
	FVector zerovec(0, 0, 0);

	//spawning platform in
	AActor* platform;
	if (type == 0) {
		platform = (Aspinner*)GetWorld()->SpawnActor(Aspinner::StaticClass(), &zerovec);
		UE_LOG(LogTemp, Warning, TEXT("spinner wall length: %d"), length);
		float startY = time * timeMod;

		//end of the wall
		float endY = startY + length * timeMod;

		//middle of the wall (used for location)
		float yPos = (startY + endY) / 2;

		//how long the wall is (used for scaling the wall)
		float wallLength = endY - startY;

		//get bounds
		FVector origin;
		FVector extent;
		platform->GetActorBounds(false, origin, extent);

		//UE_LOG(LogTemp, Warning, TEXT("wall length: %f"), wallLength);
		//UE_LOG(LogTemp, Warning, TEXT("wall extent y: %f"), extent.Y);
		UE_LOG(LogTemp, Warning, TEXT("spinner wall len / extent y: %f"), (wallLength/extent.X));
		FVector scale(1.0f, wallLength*0.5f / extent.Y, 1.0f);
		platform->SetActorRelativeScale3D(scale);
	}
	else {
		platform = (AWallActor*)GetWorld()->SpawnActor(AWallActor::StaticClass(), &zerovec);

		UE_LOG(LogTemp, Warning, TEXT("slider wall length: %d"), length);
		float startY = time * timeMod;

		//end of the wall
		float endY = startY + length * timeMod;

		//middle of the wall (used for location)
		float yPos = (startY + endY) / 2;

		//how long the wall is (used for scaling the wall)
		float wallLength = endY - startY;

		//get bounds
		FVector origin;
		FVector extent;
		platform->GetActorBounds(false, origin, extent);

		//UE_LOG(LogTemp, Warning, TEXT("wall length: %f"), wallLength);
		//UE_LOG(LogTemp, Warning, TEXT("wall extent y: %f"), extent.Y);
		UE_LOG(LogTemp, Warning, TEXT("slider len / extent y: %f"), (wallLength / extent.Y));
		FVector scale(1.0f, wallLength * 0.5f / extent.Y, 1.0f);
		platform->SetActorRelativeScale3D(scale);
		
	}
	

	//start of the wall
	float startY = time * timeMod;

	//end of the wall
	float endY = startY + length * timeMod;

	//middle of the wall (used for location)
	float yPos = (startY + endY) / 2;

	//how long the wall is (used for scaling the wall)
	float wallLength = endY - startY;

	//get bounds
	FVector origin;
	FVector extent;
	platform->GetActorBounds(false, origin, extent);

	//UE_LOG(LogTemp, Warning, TEXT("wall length: %f"), wallLength);
	//UE_LOG(LogTemp, Warning, TEXT("wall extent y: %f"), extent.Y);
	//UE_LOG(LogTemp, Warning, TEXT("wall len / extent y: %f"), (wallLength/extent.Y));
	//setting scale
	


	

	//setting xval (how far left and right to jump)
	//float xval = FMath::FRandRange(-200, 200);

	



	//set location in world
	platform->SetActorLocation(FVector(xpos, yPos, 0));

	//location += FVector(xval, PlatformYSpacing + extent.Y / 2, PlatformZSpacing);
	return platform;
}



void ULevelGenerator::GeneratePlatforms() {
	using namespace osu;
	Beatmap beats(ToU);
	int count = 1;
	for (const auto& hitobj : beats.HitObjects())
	{

		//UE_LOG(LogTemp, Warning, TEXT("Hello"));
		//UE_LOG(LogTemp, Warning, TEXT("The float value is: %d"), hitobj.Time());

		
		float xpos = FMath::Sin(hitobj.Time() / 100.0f);
		
		if (hitobj.Type() == HitObjectType::CIRCLE) 
		{
			ConstructPlatform(2 * xpos, hitobj.Time() * timeMod);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("The float value is!!!!!!: %d"), hitobj.Length());
			if (hitobj.Type() == HitObjectType::SPINNER) {
				UE_LOG(LogTemp, Warning, TEXT("count: %d"), count);
				ConstructWall(2.5 * xpos, hitobj.Time(), hitobj.Length(), 0);
				count++;
			}
			else
			{
				ConstructWall(2.5 * xpos, hitobj.Time(), hitobj.Length(), 1);
			}
			
			
		}
		
	}
}


// Called when the game starts
void ULevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	//play music from specified path
	Sound = LoadObject<USoundBase>(NULL, TEXT("/Game/Songs/NeverGetUsedToPeople"));
	UGameplayStatics::PlaySound2D(this, Sound);

	timeMod = 2;
	

	GeneratePlatforms();

}





// Called every frame
void ULevelGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	timer += DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("time = %f"), timer);
	using namespace osu;
	Beatmap beats(ToU);



	bool audioplaying = false;
	float audiotime = 0;

	if (!audioplaying) {
		audioplaying = true;
	}

		int index = 0;
		while (index < beats.HitObjects().size() - 1 && beats.HitObjects()[index + 1].Time() <= 1000 * audiotime) {
			index++;
		}
		if (index < beats.HitObjects().size() - 1) {
			float ypos = 0;
			if (1000 * audiotime > beats.HitObjects()[0].Time() / 1000)
			{
				float currentSZ = beats.HitObjects()[index].Time() * timeMod;
				float nextSZ = beats.HitObjects()[index + 1].Time() * timeMod;
				float currentEZ = currentSZ + beats.HitObjects()[index].Length() * timeMod;
				float nextEZ = nextSZ + beats.HitObjects()[index + 1].Length() * timeMod;
				float currentZ = (currentEZ + currentSZ) / 2;
				float nextZ = (nextEZ + nextSZ) / 2;


				//FVector PlayerVelocity(0, 0, 0);
				//FVector currentLocation(0, currentEZ, 0);
				//FVector nextLocation(0, nextSZ, 0);

				//bool check = UGameplayStatics::SuggestProjectileVelocity(this, PlayerVelocity, currentLocation, nextLocation, walkSpeed);

				//UE_LOG(LogTemp, Warning, TEXT("The float value is!!!!!!: %b"), check);

			}
		}

}


