// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PingPongGoal.generated.h"

UCLASS()
class PINGPONG_API APingPongGoal : public AActor
{
	GENERATED_BODY()

	protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BodyCollision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	public:
	//////////// lesson 4 ///////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	uint8 id = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SCORE")
	int32 Score = 0;

	UFUNCTION(BlueprintCallable)
		int32 GetScore();
	
	UFUNCTION(BlueprintCallable)
		uint8 GetId();
	
	//////////// lesson 4 ///////////////////
public:	
	// Sets default values for this actor's properties
	APingPongGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
