// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGoal.h"



// Sets default values
APingPongGoal::APingPongGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Platfrom Body Collider"));
	SetRootComponent(BodyCollision);
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Body	Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	SetReplicates(true);
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void APingPongGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APingPongGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 APingPongGoal::GetScore()
{
	return Score;
}

uint8 APingPongGoal::GetId()
{
	return  id;
}

