// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "osu.hpp"
#include <map>
#include <time.h>
#include <string>
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LevelGenerator.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class QUANTUMLEAP_API ULevelGenerator : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
	float walkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
	float zPos = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
	float playerY = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
	float jumpZVelocity = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
	FString chosensong = FString(TEXT("ToU"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes)
	TMap<FString, FString> songlist;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> songlistArray;
	UPROPERTY(EditAnywhere, Category = "Music")
	class USoundBase *ss;
	UPROPERTY(EditAnywhere, Category = "Music")
	class USoundBase *tu;
	UPROPERTY(EditAnywhere, Category = "Music")
	class USoundBase *song;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attributes)
	bool jump = false;

	ULevelGenerator();
	void GeneratePlatforms();
	IOnlineSessionPtr OnlineSessionInterface;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Called every frame
	float timeMod;
	float timer = 0;
	AActor *ConstructPlatform(float xpos, int time);
	AActor *ConstructWall(float xpos, int time, int length, int type);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	std::vector<AActor *> platforms;
	float platformYSpacing = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	UAudioComponent *MusicAudioComponent;
	bool left = false;
};
