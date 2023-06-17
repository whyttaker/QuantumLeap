// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGenerator.h"
#include "osu.hpp"
#include "ToU.hpp"
#include "PlatformActor.h"
#include "WallActor.h"
#include "spinner.h"
#include "memphis.hpp"
#include "OnlineSessionSettings.h"

#include "Kismet/GameplayStatics.h" /*
#include "Sound/SoundCue.h"
#include "Sound/Soundbase.h"
#include "Components/AudioComponent.h"*/

osu::Beatmap beats(ToU);
// Sets default values for this component's properties
ULevelGenerator::ULevelGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	/*if (OnlineSubsystem) {
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Found subsystem"))
			);
		}
	}*/
}

AActor *ULevelGenerator::ConstructPlatform(float xpos, int time)
{
	// UE_LOG(LogTemp, Warning, TEXT("The float value is: %d"), time);
	FVector zerovec(0, 0, 0);

	// spawning platform in
	APlatformActor *platform = (APlatformActor *)GetWorld()->SpawnActor(APlatformActor::StaticClass(), &zerovec);

	// setting scale
	FVector scale(1.0f, 1.0f, 1.0f);
	platform->SetActorScale3D(scale);

	// setting xval (how far left and right to jump)
	/*float xval = FMath::FRandRange(-200, 200);*/
	platform->Tags.Add("Platform");
	// get bounds
	FVector origin;
	FVector extent;
	platform->GetActorBounds(false, origin, extent);
	platformYSpacing = extent.Y / 2;

	// set location in world
	FVector location(xpos, time, -extent.Z / 2 + 10.0f);
	platform->SetActorLocation(location);

	// location += FVector(xval, PlatformYSpacing + extent.Y / 2, PlatformZSpacing);
	platforms.push_back(platform);
	return platform;
}

AActor *ULevelGenerator::ConstructWall(float xpos, int time, int length, int type)
{
	using namespace osu;
	// UE_LOG(LogTemp, Warning, TEXT("wall length: %d"), length);
	FVector zerovec(0, 0, 0);
	
	// spawning platform in
	AActor *platform;
	if (type == 0)
	{
		platform = (Aspinner *)GetWorld()->SpawnActor(Aspinner::StaticClass(), &zerovec);
		//UE_LOG(LogTemp, Warning, TEXT("spinner wall length: %d"), length);
		float startY = time * timeMod;

		// end of the wall
		float endY = startY + length * timeMod;

		// middle of the wall (used for location)
		float yPos = (startY + endY) / 2;

		// how long the wall is (used for scaling the wall)
		float wallLength = endY - startY;

		// get bounds
		FVector origin;
		FVector extent;
		platform->GetActorBounds(false, origin, extent);

		// UE_LOG(LogTemp, Warning, TEXT("wall length: %f"), wallLength);
		// UE_LOG(LogTemp, Warning, TEXT("wall extent y: %f"), extent.Y);
		FVector scale(1.0f, wallLength * 0.5f / extent.Y, 4.0f);
		platform->SetActorRelativeScale3D(scale);
		platform->Tags.Add("Wall");
	}
	else
	{
		platform = (AWallActor *)GetWorld()->SpawnActor(AWallActor::StaticClass(), &zerovec);

		//UE_LOG(LogTemp, Warning, TEXT("slider wall length: %d"), length);
		float startY = time * timeMod;

		// end of the wall
		float endY = startY + length * timeMod;

		// middle of the wall (used for location)
		float yPos = (startY + endY) / 2;

		// how long the wall is (used for scaling the wall)
		float wallLength = endY - startY;

		// get bounds
		FVector origin;
		FVector extent;
		platform->GetActorBounds(false, origin, extent);

		// UE_LOG(LogTemp, Warning, TEXT("wall length: %f"), wallLength);
		// UE_LOG(LogTemp, Warning, TEXT("wall extent y: %f"), extent.Y);
		// UE_LOG(LogTemp, Warning, TEXT("slider len / extent y: %f"), (wallLength / extent.Y));
		FVector scale(1.0f, wallLength * 0.5f / extent.Y, 4.0f);
		platform->SetActorRelativeScale3D(scale);
		platform->Tags.Add("Wall");
	}
	platform->Tags.Add("Wall");

	// start of the wall
	float startY = time * timeMod;

	// end of the wall
	float endY = startY + length * timeMod;

	// middle of the wall (used for location)
	float yPos = (startY + endY) / 2;

	// how long the wall is (used for scaling the wall)
	float wallLength = endY - startY;

	// get bounds
	FVector origin;
	FVector extent;
	platform->GetActorBounds(false, origin, extent);

	// UE_LOG(LogTemp, Warning, TEXT("wall length: %f"), wallLength);
	// UE_LOG(LogTemp, Warning, TEXT("wall extent y: %f"), extent.Y);
	// UE_LOG(LogTemp, Warning, TEXT("wall len / extent y: %f"), (wallLength/extent.Y));
	// setting scale

	// setting xval (how far left and right to jump)
	// float xval = FMath::FRandRange(-200, 200);

	// set location in world
	if(left){
		platform->SetActorLocation(FVector(xpos, yPos, 0));
	}else{
		platform->SetActorLocation(FVector(-xpos, yPos, 0));
	}
	left = !left;

	// location += FVector(xval, PlatformYSpacing + extent.Y / 2, PlatformZSpacing);
	platforms.push_back(platform);
	return platform;
}

void ULevelGenerator::GeneratePlatforms()
{
	using namespace osu;
	int count = 1;
	for (auto &hitobj : beats.HitObjects())
	{

		// UE_LOG(LogTemp, Warning, TEXT("Hello"));
		// UE_LOG(LogTemp, Warning, TEXT("The float value is: %d"), hitobj.Time());

		float xpos = FMath::Sin(hitobj.Time() / 100.0f);
		float wallxpos = 250;
		if (hitobj.Type() == HitObjectType::CIRCLE)
		{
			ConstructPlatform(100 * xpos, hitobj.Time() * timeMod);
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("The float value is!!!!!!: %d"), hitobj.Length());
			if (hitobj.Type() == HitObjectType::SPINNER)
			{
				//UE_LOG(LogTemp, Warning, TEXT("count: %d"), count);
				ConstructWall(wallxpos, hitobj.Time(), hitobj.Length(), 0);
				count++;
			}
			else
			{
				ConstructWall(wallxpos, hitobj.Time(), hitobj.Length(), 1);
			}
		}
	}
}

// Called when the game starts
void ULevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	// play music from specified path
	Sound = LoadObject<USoundBase>(NULL, TEXT("/Game/Songs/ToU"));

	timeMod = walkSpeed / 1000.0f;
	GeneratePlatforms();
	//UGameplayStatics::PlaySound2D(this, Sound);
	timer = 0;
}

// Called every frame
void ULevelGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	jump = false;
	timer += DeltaTime;
	// UE_LOG(LogTemp, Warning, TEXT("time = %f"), timer);
	using namespace osu;
	

	bool audioplaying = false;

	if (!audioplaying)
	{
		audioplaying = true;

		playerY = timer * walkSpeed;
		int index = 0;
		while (index < beats.HitObjects().size() - 1 && beats.HitObjects()[index + 1].Time() <= 1000 * timer)
		{
			index++;
		}
		
		
		


		if (index < beats.HitObjects().size() - 1)
		{
			if (1000 * timer > beats.HitObjects()[0].Time())
			{
				
				float currentSY = beats.HitObjects()[index].Time() * timeMod;
				float nextSY = beats.HitObjects()[index + 1].Time() * timeMod;
				float currentEY = currentSY + beats.HitObjects()[index].Length() * timeMod;
				float nextEY = nextSY + beats.HitObjects()[index + 1].Length() * timeMod;
				float currentY = (currentEY + currentSY) / 2;
				float nextY = (nextEY + nextSY) / 2;
				float startPos = currentEY;
				float endPos = nextSY;
				float distance = endPos - startPos;
				float pY = (timer * walkSpeed) - startPos;
				float grav = -980;
				jumpZVelocity = -grav * distance / 4 / 1000;
				if (pY > 0 && pY < distance)
				{
					// float half = distance / 2;
					// float grav = -0.01f;
					// float halfZ = 0.5f * grav * (half * half - distance * half);
					// if (halfZ > 1000)
					// {
					// 	grav = 1000 / (0.5f * (half * half - distance * half));
					// }

					zPos = grav * (pY * pY - distance * pY) / 1000000 + 100;
					//UE_LOG(LogTemp, Warning, TEXT("zPos = %f, pY = %f, distance = %f"), zPos, pY, distance);

					// FVector zerovec(0, 0, 0);

					// // spawning platform in
					// APlatformActor *platform = (APlatformActor *)GetWorld()->SpawnActor(APlatformActor::StaticClass(), &zerovec);

					// // setting scale
					// FVector scale(0.1f, 0.1f, 0.1f);
					// platform->SetActorScale3D(scale);

					// // setting xval (how far left and right to jump)
					// /*float xval = FMath::FRandRange(-200, 200);*/
					// // get bounds
					// FVector origin;
					// FVector extent;
					// platform->GetActorBounds(false, origin, extent);
					// platformYSpacing = extent.Y / 2;

					// // set location in world
					// platform->SetActorLocation({platformYSpacing, timer * walkSpeed, zPos});
				}
			}

			if(!beats.HitObjects()[index].isHit()){
			beats.HitObjects()[index].setHit(true);
			jump = true;
			UE_LOG(LogTemp, Warning, TEXT("jump set to true"));
			}
			// FVector PlayerVelocity(0, 0, 0);
			// FVector currentLocation(0, currentEZ, 0);
			// FVector nextLocation(0, nextSZ, 0);

			// bool check = UGameplayStatics::SuggestProjectileVelocity(this, PlayerVelocity, currentLocation, nextLocation, walkSpeed);

			// UE_LOG(LogTemp, Warning, TEXT("The float value is!!!!!!: %b"), check);
		}
	}
}
