// Fill out your copyright notice in the Description page of Project Settings.


#include "APingPongPlatform.h"

#include "Engine/AssetManager.h"

APingPongPlatform::APingPongPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Platfrom Body Collider"));
	SetRootComponent(BodyCollision);
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Body	Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	SetReplicates(true);
	SetReplicateMovement(true);
	}
// Called when the game starts or when spawned
void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();

	LoadBodyMesh();
}
// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



///////////////////// Lesson 7 ++ //////////////////////////////

void APingPongPlatform::LoadBodyMesh()
{
	FStreamableDelegate LoadMeshDelegate;
	
	LoadMeshDelegate.BindUObject(this, &APingPongPlatform::OnBodyMeshLoaded);
	
	UAssetManager& assetManager = UAssetManager::Get();
	
	FStreamableManager& streamableManager =	assetManager.GetStreamableManager();
	
	AssetHandle = streamableManager.RequestAsyncLoad(BodyMeshRef.ToStringReference(),LoadMeshDelegate);
}

void APingPongPlatform::OnBodyMeshLoaded()
{
	UStaticMesh * loadedMesh =  Cast<UStaticMesh>(AssetHandle.Get()->GetLoadedAsset());
	
    if(loadedMesh)   
    {
    BodyMesh->SetStaticMesh(loadedMesh);
    }

}
///////////////////// Lesson 7 --//////////////////////////////




bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return true;
}
void APingPongPlatform::Server_MoveRight_Implementation(float AxisValue)
{
	if(AxisValue != 0)
	{
		UE_LOG(LogTemp, Warning,
		TEXT("APingPongPlatform::Server_MoveRight_Implementation"));
	}
	FVector currLocation = GetActorLocation();
	FVector nextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed *
	AxisValue;
	if(!SetActorLocation(nextLocation, true))
	{
		SetActorLocation(currLocation);
	}
}


