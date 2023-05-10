// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "spinner.generated.h"

UCLASS()
class QUANTUMLEAP_API Aspinner : public AActor
{
	GENERATED_BODY()
		UStaticMeshComponent* VisualMesh;
	
public:	
	// Sets default values for this actor's properties
	Aspinner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
