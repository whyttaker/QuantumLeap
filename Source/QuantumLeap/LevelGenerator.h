// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "osu.hpp"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "LevelGenerator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUANTUMLEAP_API ULevelGenerator : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		float PlatformYSpacing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		float PlatformZSpacing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		float WallSpacing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		float NumberOfBeats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
		float walkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		FString Name;
	UPROPERTY(EditAnywhere, Category = "Music")
		class USoundBase* Sound;

	ULevelGenerator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	// Called every frame
	float timeMod;
	float timer = 0;
	AActor* ConstructPlatform(float xpos, int time);
	AActor* ConstructWall(float xpos, int time, int length, int type);
	void GeneratePlatforms();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
		UAudioComponent* MusicAudioComponent;
		
};
