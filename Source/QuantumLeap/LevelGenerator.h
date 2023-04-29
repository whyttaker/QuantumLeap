// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
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
	ULevelGenerator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
