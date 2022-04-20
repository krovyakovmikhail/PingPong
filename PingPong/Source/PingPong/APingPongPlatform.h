// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "APingPongPlatform.generated.h"


UCLASS()
class PINGPONG_API APingPongPlatform : public AActor
{
	GENERATED_BODY()
	protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent * BodyCollision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100;

	///////////////////// Lesson 7 ++ //////////////////////////////
	TSharedPtr<FStreamableHandle> AssetHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSoftObjectPtr<UStaticMesh> BodyMeshRef;
	
	/////////////////////// Lesson 7 -- ////////////////////////////

	
	public:
	// Sets default values for this actor's properties
	APingPongPlatform();
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float AxisValue);
	
	///////////////////// Lesson 7 ++ //////////////////////////////
	void LoadBodyMesh();
	void OnBodyMeshLoaded();
	/////////////////////// Lesson 7 -- ////////////////////////////
};
