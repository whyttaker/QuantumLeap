// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformActor.generated.h"

UCLASS()
class QUANTUMLEAP_API APlatformActor : public AActor
{
	GENERATED_BODY()
		UStaticMeshComponent* VisualMesh;
	
public:	
	// Sets default values for this actor's properties
	APlatformActor();
	AActor ConstructPlatform(float xpos, float time);
	AActor ConstructWall(float xpos, float time);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
